/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2010-2011, Arne Welzel, <arne.welzel@googlemail.com>
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


// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <cstdlib>
#include <list>


using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;

static STRP mdNs("myns", "http://mynamespace.com");

class SingleValueMetadata : public BasicXmlMarshalable {
public:
	SingleValueMetadata() :
		BasicXmlMarshalable("simpleMetadata", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class MultiValueMetadata : public BasicXmlMarshalable {
public:
	MultiValueMetadata() :
		BasicXmlMarshalable("simpleMetadata", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class SingleValueMetadata2 : public BasicXmlMarshalable {

public:
	SingleValueMetadata2() :
		BasicXmlMarshalable("simpleMetadata2", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class MultiValueMetadata2 : public BasicXmlMarshalable {
public:
	MultiValueMetadata2() :
		BasicXmlMarshalable("simpleMetadata2", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};


static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 5)
		usage(argv[0]);

	// create ssrc object which is used for synchronous communication
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);
	XmlMarshalable *single = new SingleValueMetadata();
	XmlMarshalable *multi = new MultiValueMetadata();
	XmlMarshalable *single2 = new SingleValueMetadata2();
	XmlMarshalable *multi2 = new MultiValueMetadata2();
	IpAddress *i1 = IpAddress::createIpv4Address("192.168.0.1");

	SubPublish *sub1 = PublishUpdate::createPublishUpdate(single, i1->clone(), session);
	SubPublish *sub2 = PublishUpdate::createPublishUpdate(multi, i1->clone(), session);
	SubPublish *sub3 = PublishUpdate::createPublishUpdate(multi2, i1->clone(), session);
	SubPublish *sub4 = PublishUpdate::createPublishUpdate(single2, i1, session);

	PublishRequest *pr1 = PublishRequest::createPublishRequest(sub1);
	PublishRequest *pr2 = PublishRequest::createPublishRequest(sub2);
	PublishRequest *pr3 = PublishRequest::createPublishRequest(sub3);
	PublishRequest *pr4 = PublishRequest::createPublishRequest(sub4);

	try {
		cout << "Doing newSession\t";
		ssrc->newSession();
		cout << "Ok" << endl;
		cout << "Doing publish1\t\t";
		ssrc->publish(pr1);
		cout << "Ok" << endl;
		cout << "Doing publish2\t\t";
		try {
			ssrc->publish(pr2);
			cout << "Should have failed..." << endl;
		} catch (ErrorResultError e) {
			cout << "Ok (failed on purpose)" << endl;;
		}

		cout << "Doing publish3\t\t";
		ssrc->publish(pr3);
		cout << "Ok" << endl;
		cout << "Doing publish1\t\t";
		try {
			ssrc->publish(pr4);
			cout << "Should have failed..." << endl;
		} catch (ErrorResultError e) {
			cout << "Ok (failed on purpose)" << endl;;
		}

		cout << "Doing endSession\t";
		ssrc->endSession();
		cout << "Ok" << endl;

	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "ErrorResult:" << endl;
		cerr << " " << e.getErrorCodeString() << endl;
		cerr << " " << e.getErrorString() << endl;
	} catch (...) {
		cerr << "Uncatched Exception occured" << endl;
		throw;
	}

	// delete the publish requests and all their childs
	delete pr1; delete pr2; delete pr3; delete pr4;
	
	// this closes the TCP connection
	delete ssrc;
	return 0;
}
