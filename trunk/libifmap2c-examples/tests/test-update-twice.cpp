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
 *
 * There was a bug reported in irond 0.2.0, reproduce it...
 */


// libifmap2c includes
#include <libifmap2c/requests.h>
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <cstdlib>
#include <list>

#include "common.h"


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
	XmlMarshalable *single = new SingleValueMetadata();
	XmlMarshalable *ipmac = Metadata::createIpMac();
	IpAddress *ip = Identifiers::createIPv4("192.168.1.11");
	MacAddress *mac = Identifiers::createMac("00:11:22:33:44:55");

	PublishElement *up1 = Requests::createPublishUpdate(ipmac->clone(),
			ip->clone(),
			session,
			mac->clone());
	PublishElement *up2 = Requests::createPublishUpdate(single->clone(), ip->clone());
	PublishElement *up3 = Requests::createPublishUpdate(single, ip->clone());
	PublishElement *del = Requests::createPublishDelete(FILTER_MATCH_ALL,
			ip->clone(), mac->clone());
	PublishElement *up4 = Requests::createPublishUpdate(ipmac, ip,
			session,
			mac);

	list<PublishElement *> list;
	list.push_back(up2);
	list.push_back(up3);
	list.push_back(del);
	list.push_back(up4);

	PublishRequest *pr1 = Requests::createPublishReq(up1);
	PublishRequest *pr2 = Requests::createPublishReq(list);
	pr1->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	pr2->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	try {
		ssrc->newSession();
		ssrc->publish(pr1);
		ssrc->publish(pr2);
		ssrc->endSession();
	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete pr1, delete pr2;
	
	// this closes the TCP connection
	delete ssrc;
	return 0;
}
