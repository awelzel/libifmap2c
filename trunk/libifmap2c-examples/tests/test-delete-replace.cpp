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
#include <libifmap2c/requests.h>
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <cstdlib>
#include <list>

#include "common.h"
#include "testcommon.h"


using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;

static STRP mdNs("myns", "http://mynamespace.com");

class SingleValueMetadata : public BasicXmlMarshalable {
public:
	SingleValueMetadata(const string &value) :
		BasicXmlMarshalable("simpleMetadata", "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
		addXmlChild(
			new BasicXmlMarshalable("value", value, NO_NSPAIR)
		);
	}
};

static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

static int
checkMdValue(SearchResult *sres, const string &val)
{
	ResultItem *ri = sres->getResultItems().front();
	if (ri == NULL)
		return 1;
	XmlMarshalable *md = ri->getMetadata().front();
	
	if (md == NULL)
		return 1;
	XmlMarshalable *valEl = md->getXmlChildren().front();
	
	if (valEl == NULL)
		return 1;

	string foundVal = valEl->getXmlElementValue();
	return foundVal.compare(val);
}

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	SSRC *ssrc = SSRC::createSSRC(url, user, pass, capath);
	ARC *arc = ssrc->getARC();
	XmlMarshalable *single1 = new SingleValueMetadata("1");
	XmlMarshalable *single2 = new SingleValueMetadata("2");
	IpAddress *ip = Identifiers::createIPv4("192.168.1.11");
	MacAddress *mac = Identifiers::createMac("00:11:22:33:44:55");
	PollResult *pres;

	PublishElement *up1 = Requests::createPublishUpdate(single1,
			ip->clone(),
			session,
			mac->clone());

	PublishElement *del = Requests::createPublishDelete(
			FILTER_MATCH_ALL, ip->clone(), mac->clone());
	PublishElement *up2 = Requests::createPublishUpdate(single2,
			ip->clone(),
			session,
			mac->clone());

	list<PublishElement *> list;
	list.push_back(del);
	list.push_back(up2);

	PublishRequest *pr1 = Requests::createPublishReq(up1);
	PublishRequest *pr2 = Requests::createPublishReq(list);

	SubscribeUpdate *su = Requests::createSubscribeUpdate(
			"sub1",
			FILTER_MATCH_ALL,
			10,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			ip);
	
	SubscribeRequest *sr = Requests::createSubscribeReq(su);

	try {
		ssrc->newSession();

		ssrc->subscribe(sr);

		pres = arc->poll();
		
		if (!(cntAll(pres) > 0))
			cerr << "no results first poll" << endl;

		if (cntAll(pres) != cntSe(pres))
			cerr << "non search-result first poll" << endl;
		
		delete pres;
		
		ssrc->publish(pr1);
		
		pres = arc->poll();
		if (cntAll(pres) != cntUp(pres)) 
			cerr << "second poll - non-update result" << endl;
		
		if (cntRi(pres, ip, mac, UPDATE) != 1)
			cerr << "metadata not found" << endl;
		
		if (checkMdValue(pres->getUpdateResults().front(), "1"))
			cerr << "wrong metadata found" << endl;

		delete pres;


		// The old metadata gets deleted and replaced after that.
		// Atomically this means only replacing the old metadata
		ssrc->publish(pr2);
		
		pres = arc->poll();
		if (cntAll(pres) != cntUp(pres)) 
			cerr << "second poll - non-update result" << endl;
		
		if (cntRi(pres, ip, mac, UPDATE) != 1)
			cerr << "metadata not found" << endl;

		if (checkMdValue(pres->getUpdateResults().front(), "2"))
			cerr << "wrong metadata found" << endl;

		delete pres;
		ssrc->endSession();
	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete pr1, delete pr2;
	
	// this closes the TCP connection
	delete arc;
	delete ssrc;
	return 0;
}
