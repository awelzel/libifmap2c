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

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"

// make life easier
using namespace ifmap2c;
using namespace std;

static void usage(const char *prog)
{
	cerr << "usage: " << prog << " myself|<some-publisher-id>"
		INDEPENDENT_USAGE_STRING << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	char *pubId;
	char *url, *user, *pass, *capath;
	url = user = pass = capath = NULL;

	if (argc != 6 && argc != 2) {
		usage(argv[0]);
	}

	pubId = argv[1];
	
	if (argc == 6) {
		loadCmdParameters(&argv[2], &url, &user, &pass, &capath);
	} else {
		loadEnvParameters(&url, &user, &pass, &capath);
		
		if (!url || !user || !pass || !capath) {
			cerr << "Environment variables not set?\n\n";
			usage(argv[0]);
		}
	}

	SSRC *ssrc = SSRC::createSSRC(url, user, pass, capath);


	try {
		cout << "Doing newSession... ";
		ssrc->newSession();
		cout << "Ok! SessionID=\"" << ssrc->getSessionId() << "\"";
		cout << " PublisherID=\"" << ssrc->getPublisherId() << "\"" << endl;
		
		if (!strcmp(pubId, "myself")) {
			cout << "Purging with publisher-id=\"" 
				<< ssrc->getPublisherId() << "\"... ";
			ssrc->purgePublisher();
		} else {
			cout << "Purging with publisher-id=\"" 
				<< pubId << "\"... ";
			ssrc->purgePublisher(pubId);
		}
		cout << "Ok!" << endl;;
		cout << "Doing endSession... ";
		ssrc->endSession();
		cout << "Ok!" << endl;

		// catch some possible errors
	} catch (CommunicationError e) {
		cerr << "CommunicationError: ";
		cerr << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "IF-MAP ErrorResult" << endl;
		cerr << "   " << e.getErrorCodeString() << endl;
		cerr << "   " << e.getErrorString() << endl;
	} catch (XmlMarshalError e) {
		cerr << "XmlMarshalError: ";
		cerr << e.getMessage() << endl;
	} catch (XmlUnmarshalError e) {
		cerr << "XmlUnmarshalError: ";
		cerr << e.getMessage() << endl;
	}

	// delete the ssrc
	delete ssrc;

	return 0;
}
