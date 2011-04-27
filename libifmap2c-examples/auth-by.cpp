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
	cerr << "usage: " << prog << " update|delete device arname"
			" ifmap-server-url user password capath" << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	if (argc != 8) {
		usage(argv[0]);
	}

	char* op = argv[1];
	if (strcmp(op, "update") != 0 && strcmp(op, "delete") != 0) {
		usage(argv[0]);
	}

	char* devArg = argv[2];
	char* arArg = argv[3];
	char* url = argv[4];
	char* user = argv[5];
	char* password = argv[6];
	char *capath = argv[7];

	SSRC  *ssrc = SSRC::createSSRC(url, user, password, capath);
	PublishRequest *pubReq = NULL;
	SubPublish *subReq = NULL;
	XmlMarshalable *authby = NULL;

	Identifier *dev = Identifiers::createDev(devArg);
	Identifier *ar = Identifiers::createAr(arArg);

	if (strcmp(op, "update") == 0) {
		authby = Metadata::createAuthBy();
		subReq = Requests::createPublishUpdate(authby, dev, forever, ar);
	} else {
		subReq = Requests::createPublishDelete("meta:authenticated-by", dev, ar);
	}

	// create the publish request
	pubReq = Requests::createPublishReq(subReq);

	// declare the default meta namespace on the publish element
	// it's not there by default
	pubReq->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	// no need to delete those, will be done when pubReq is deleted
	subReq = NULL;
	authby = NULL;
	ar = NULL; dev = NULL;


	try {
		cout << "Doing newSession... ";
		ssrc->newSession();
		cout << "Ok! SessionID=\"" << ssrc->getSessionId() << "\"";
		cout << " PublisherID=\"" << ssrc->getPublisherId() << "\"" << endl;
		cout << "Doing publish... ";
		ssrc->publish(pubReq);
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

	// delete the request and all childs that have been added
	delete pubReq;

	// delete the ssrc
	delete ssrc;

	return 0;
}
