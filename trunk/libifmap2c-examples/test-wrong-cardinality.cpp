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

class SimpleSingleMetadata : public BasicXmlMarshalable {
public:
	SimpleSingleMetadata() :
		BasicXmlMarshalable("simpleMetadata", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class SimpleMultiMetadata : public BasicXmlMarshalable {
public:
	SimpleMultiMetadata() :
		BasicXmlMarshalable("simpleMetadata", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class SimpleSingleMetadata2 : public BasicXmlMarshalable {
public:
	SimpleSingleMetadata2() :
		BasicXmlMarshalable("simpleMetadata2", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class SimpleMultiMetadata2 : public BasicXmlMarshalable {
public:
	SimpleMultiMetadata2() :
		BasicXmlMarshalable("simpleMetadata2", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class SingleIpMacMetadata : public BasicXmlMarshalable {
public:
	SingleIpMacMetadata() :
		BasicXmlMarshalable("ip-mac", "", TCG_META_NSPAIR)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(TCG_META_NSPAIR);
	}
};

class MultiDeviceIpMetadata : public BasicXmlMarshalable {
public:
	MultiDeviceIpMetadata() :
		BasicXmlMarshalable("device-ip", "", TCG_META_NSPAIR)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(TCG_META_NSPAIR);
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
	XmlMarshalable *simplesingle = new SimpleSingleMetadata();
	XmlMarshalable *simplemulti = new SimpleMultiMetadata();
	XmlMarshalable *simplesingle2 = new SimpleSingleMetadata2();
	XmlMarshalable *simplemulti2 = new SimpleMultiMetadata2();
	XmlMarshalable *singleipmac = new SingleIpMacMetadata();
	XmlMarshalable *multidevip = new MultiDeviceIpMetadata();

	Device *dev = Identifiers::createDev("bloodyDevice");
	IpAddress *ip = Identifiers::createIPv4("192.168.0.1");
	MacAddress *mac = Identifiers::createMac("00:11:22:33:44:55");



	PublishElement *up1 = Requests::createPublishUpdate(simplesingle,
			dev->clone());
	PublishElement *up2 = Requests::createPublishUpdate(simplemulti,
			ip->clone());
	PublishElement *up3 = Requests::createPublishUpdate(simplemulti2,
			ip->clone());
	PublishElement *up4 = Requests::createPublishUpdate(simplesingle2,
			dev->clone());
	PublishElement *up5 = Requests::createPublishUpdate(singleipmac,
			mac, session, ip->clone());
	PublishElement *up6 = Requests::createPublishUpdate(multidevip,
			dev, session, ip);


	PublishRequest *pr1 = PublishRequest::createPublishRequest(up1);
	PublishRequest *pr2 = PublishRequest::createPublishRequest(up2);
	PublishRequest *pr3 = PublishRequest::createPublishRequest(up3);
	PublishRequest *pr4 = PublishRequest::createPublishRequest(up4);
	PublishRequest *pr5 = PublishRequest::createPublishRequest(up5);
	PublishRequest *pr6 = PublishRequest::createPublishRequest(up6);

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
			cout << "Ok (failed on purpose) [" << e << "]" << endl;
		}
		
		cout << "Doing publish3\t\t";
		ssrc->publish(pr3);
		cout << "Ok" << endl;
		cout << "Doing publish4\t\t";
		try {
			ssrc->publish(pr4);
			cout << "Should have failed..." << endl;
		} catch (ErrorResultError e) {
			cout << "Ok (failed on purpose) [" << e << "]" << endl;
		}
		
		cout << "Doing publish5\t\t";
		try {
			ssrc->publish(pr5);
			cout << "Should have failed..." << endl;
		} catch (ErrorResultError e) {
			cout << "Ok (failed on purpose) [" << e << "]" << endl;
		}
		
		cout << "Doing publish6\t\t";
		try {
			ssrc->publish(pr6);
			cout << "Should have failed..." << endl;
		} catch (ErrorResultError e) {
			cout << "Ok (failed on purpose) [" << e << "]" << endl;
		}

		cout << "Doing endSession\t";
		ssrc->endSession();
		cout << "Ok" << endl;

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}

	// delete the publish requests and all their childs
	delete pr1; delete pr2; delete pr3; delete pr4;
	delete pr5; delete pr6;
	
	// this closes the TCP connection
	delete ssrc;
	return 0;
}
