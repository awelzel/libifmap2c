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


// make life easier
using namespace ifmap2c;
using namespace std;

static void usage(const char *prog)
{
	cerr << "usage: " << prog << " myself|<some-publisher-id>"
			" ifmap-server-url user password capath" << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	if (argc != 6) {
		usage(argv[0]);
	}

	char* pubId = argv[1];
	char* url = argv[2];
	char* user = argv[3];
	char* password = argv[4];
	char *capath = argv[5];

	SSRC  *ssrc = SSRC::createSSRC(url, user, password, capath);


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
