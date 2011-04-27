/*
 * Copyright 2008 Juniper Networks, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * o Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the  
 *   distribution.
 * o Neither the name of Juniper Networks nor the names of its
 *   contributors may be used to endorse or promote products 
 *   derived from this software without specific prior written 
 *   permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * 2010-10-10 Arne Welzel: Modified in order to work with libifmap2c
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
