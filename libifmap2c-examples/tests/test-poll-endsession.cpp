/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2011, Arne Welzel, <arne.welzel@googlemail.com>
 *
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright
 * notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder shall not
 * be used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization of the copyright holder.
 */


/*
 * Check if running endSession while a poll is pending lead to an
 * EndSessionResult Exception.
 */

#include <iostream>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"

extern "C" {
	#include <pthread.h>
	#include <unistd.h>
}

using namespace std;
using namespace ifmap2c;

static void
usage(const char *const name)
{
	cerr << "usage: " << name << INDEPENDENT_USAGE_STRING << endl;
}


static void *
pollThreadFunc(void *arg)
{
	ARC *arc = (ARC *)arg;
	try {
		arc->poll();
	} catch (EndSessionResult e) {
		return (void*)0;
	} catch (IfmapError e) {
		cerr << "[pollThread] " << e << endl;
	} catch (ErrorResultError e) {
		cerr << "[pollThread] " << e << endl;
	}
	cerr << "[pollThread] Should never end up here" << endl;
	return (void*)1;
}

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;
	pthread_t pollThread;
	int ret;
	void *tRet;

	SSRC *ssrc = NULL;
	ARC *arc = NULL;
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();


	try {
		ssrc->newSession();
		ret = pthread_create(&pollThread, NULL, pollThreadFunc, (void*)arc);
		if (ret) {
			cerr << "[mainThread] Could not start pollThread" << endl;
			return 1;
		}
		// main thread  sleeps 100 msec, this may not work
		// on other systems than Linux. usleep(3) on linux
		// says it will makes the calling thread sleep
		usleep(100000);
		ssrc->endSession();
		pthread_join(pollThread, &tRet);

	} catch (IfmapError e) {
		cerr << "[mainThread] " << e << endl;
	} catch (ErrorResultError e) {
		cerr << "[mainThread] " << e << endl;
	}
	if (tRet) {
		cerr << "[ERROR] No EndSessionResult";
		cerr << " on ARC while polling" << endl;
	}

	delete arc;
	delete ssrc;
	return 0;
}
