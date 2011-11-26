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
 * Check if the PollResults contains the newly added subgraph, if
 * one is added while doing a publish update.
 */

#include <iostream>
#include <string>
#include <list>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"
#include "testcommon.h"

using namespace std;
using namespace ifmap2c;

typedef list<SearchResult *> SRLIST;
typedef SRLIST::iterator SRLISTIT;
typedef list<ResultItem *> RILIST;
typedef RILIST::iterator RILISTIT;

static char *url, *user, *pass, *capath;

static void
checkFirstPollResult(ARC *arc)
{
	PollResult *pres = arc->poll();
	AccessRequest *ar = Identifiers::createAr("AR100", user);
	int tmp;

	if (cntUp(pres) != 0 || cntDe(pres) != 0 || cntNo(pres) != 0) {
		cerr << "unexpected second poll result with non-search results" << endl;
		return;
	}

	if (cntSe(pres) != 1) {
		cerr << "unexpected initial poll result" << endl;
		return;
	}

	if ((*pres->getSearchResults().begin())->getResultItems().size() != 1) {
		cerr << "unexpected number of resultItems" << endl;
		return;
	}

	tmp = cntRi(pres, ar, SEARCH, 0);
	
	if (tmp != 1)
		cerr << "wrong #access-request RI:" << tmp << endl;

	delete ar;
	delete pres;
}

static void
checkAddedSubgraph(ARC *arc)
{
	PollResult *pres = arc->poll();
	AccessRequest *ar = Identifiers::createAr("AR100", user);
	IpAddress *ip = Identifiers::createIPv4("192.168.0.1", user);
	MacAddress *mac = Identifiers::createMac("aa:bb:cc:dd:ee:ff", user);
	int tmp1, tmp2;

	if (cntSe(pres) != 0 || cntDe(pres) != 0 || cntNo(pres) != 0) {
		cerr << "unexpected second poll result with non-update results" << endl;
		return;
	}

	if (cntUp(pres) < 1) {
		cerr << "unexpected second poll result" << endl;
		return;
	}


	tmp1 = cntRi(pres, ar, mac, UPDATE);
	tmp2 = cntRi(pres, ar, mac, UPDATE, 1);

	if (tmp1 != 1 || tmp2 != 1)
		cerr << "unexpected #RI for ar-mac link:" << tmp1  << " " << tmp2 << endl;
	
	tmp1 = cntRi(pres, ip, mac, UPDATE);
	tmp2 = cntRi(pres, ip, mac, UPDATE, 1);
	
	if (tmp1 != 1 || tmp2 != 1)
		cerr << "unexpected #RI for ip-mac link:" << tmp1  << " " << tmp2 << endl;
	
	tmp1 = cntRi(pres, ip, UPDATE);
	tmp2 = cntRi(pres, ip, UPDATE, 1);
	
	if (tmp1 != 1 || tmp2 != 1)
		cerr << "unexpected #RI for ip identifier:" << tmp1  << " " << tmp2 << endl;
	
	tmp1 = cntRi(pres, mac, UPDATE);
	tmp2 = cntRi(pres, mac, UPDATE, 0);

	// don't expect the ResultItem if no metadata is there
	if (tmp1 != 0 || tmp2 != 0)
		cerr << "unexpected #RI for mac identifier:" << tmp1  << " " << tmp2 << endl;

	delete pres;
	delete ip;
	delete ar;
	delete mac;
}

static void
usage(const char *const name)
{
	cerr << "usage: " << name << INDEPENDENT_USAGE_STRING << endl;
}

int
main(int argc, char *argv[])
{

	SSRC *ssrc = NULL;
	ARC *arc = NULL;
	PublishRequest *pr1, *pr2;
	PublishUpdate *pu;
	list<PublishElement *> pulist;
	SubscribeRequest *sr;
	SubscribeUpdate *su;
	Identifier *mac, *ip, *ar;
	XmlMarshalable *armac, *ipcap, *ipmac;
	string errsub;
	string goodsub;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	// Use user as administrative-domain to be able to run in
	// with multiple instances at once.
	ar = Identifiers::createAr("AR100", user);
	ip = Identifiers::createIPv4("192.168.0.1", user);
	mac = Identifiers::createMac("aa:bb:cc:dd:ee:ff", user);

	ipmac = Metadata::createIpMac();
	ipcap = Metadata::createCapability("dummycap");
	armac = Metadata::createArMac();
	
	pulist.push_back(Requests::createPublishUpdate(ipmac,
				ip->clone(), mac->clone()));
	pulist.push_back(Requests::createPublishUpdate(ipcap, ip));
	pu = Requests::createPublishUpdate(armac, ar->clone(), mac);
	
	pr1 = Requests::createPublishReq(pulist);
	pr2 = Requests::createPublishReq(pu);
	pr1->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	pr2->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	
	su = Requests::createSubscribeUpdate(
			"sub1",
			FILTER_MATCH_ALL,
			16,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			ar);
	
	sr = Requests::createSubscribeReq(su);

	try {	
		ssrc->newSession();
		ssrc->publish(pr1);
		ssrc->subscribe(sr);
		checkFirstPollResult(arc);
		ssrc->publish(pr2);
		checkAddedSubgraph(arc);
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sr;
	delete pr1;
	delete pr2;
	delete arc;
	delete ssrc;
	IdentifierHandlerDispatch::clearHandlers();
	IfmapRequestHandlerDispatch::clearHandlers();
	return 0;
}
