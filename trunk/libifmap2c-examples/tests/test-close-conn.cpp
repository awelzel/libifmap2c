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
 * Demonstrates that a IF-MAP session should not be closed, after the TCP
 * connection was dropped.
 */


// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <list>

#include <cstdlib>
extern "C" {
	#include <unistd.h>
}

#include "common.h"
#include "testcommon.h"

using namespace std;
using namespace ifmap2c;

// needs to be > 0
#define DISCONNECTS 1

static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	SSRC *ssrc = SSRC::createSSRC(url, user, pass, capath);

	string sessionId;

	try {
		ssrc->newSession();

		// store the session-id for later requests, as we use
		// a new SSRC instance which does not know about it
		sessionId = ssrc->getSessionId();

		for (int i = 0; i < (DISCONNECTS - 1); i++) {

			// Calling the destructor deletes the connection
			delete ssrc;
			usleep(SLEEPTIME); 

			// the session should still exists.
			// create a new SSRC
			ssrc = SSRC::createSSRC(url, user, pass, capath);

			// do a renewsession request, explicitly set the
			// sessionId, as we didn't run a newSession on this
			// SSRC.
			ssrc->renewSession(sessionId);
		}

		// Disconnect and sleep for the last time
		delete ssrc;
		usleep(SLEEPTIME); 
		
		ssrc = SSRC::createSSRC(url, user, pass, capath);

		// Ending the session
		ssrc->endSession(sessionId);
		try {
			ssrc->renewSession(sessionId);
			cerr << __FILE__ << " renewSession() worked?" << endl;
		
		} catch (const ErrorResult &e) {
			// this is expected
		}

	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	// delete the publish requests and all their childs
	delete ssrc;
	return 0;
}
