/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2010, Arne Welzel, <arne.welzel@googlemail.com>
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
#include <cstdlib>

// libifmap2c includes
#include <libifmap2c/ssrc.h>

using namespace ifmap2c;
using namespace std;

static
void usage(const char *prog) {
	cerr << "usage: " << prog << " ifmap-server-url user password capath"
			<< endl;
	exit(1);
}

int
main(int argc, char* argv[]) {
	if (argc != 5) {
		usage(argv[0]);
	}

	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3], argv[4]);
	try {
		cout << "Doing newSession... ";
		ssrc->newSession();
		cout << "Ok! SessionID=\"" << ssrc->getSessionId() << "\"";
		cout << " PublisherID=\"" << ssrc->getPublisherId() << "\""
				<< endl;
		cout << "Doing purgePublisher... ";
		ssrc->purgePublisher();
		cout << "Ok!" << endl;
		cout << "Doing endSession... ";
		ssrc->endSession();
		cout << "Ok!" << endl;

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
	} catch (ResponseParseError e) {
		cerr << "ResponseParseError: ";
		cerr << e.getMessage() << endl;
	}

	// delete the ssrc
	delete ssrc;

	return 0;
}
