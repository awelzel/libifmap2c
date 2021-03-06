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
 * Check if the PollResults contains the newly addes subgraph, if
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

static void checkMetadataThere(PollResult *pres, ResultType type, string op)
{
	AccessRequest *ar = Identifiers::createAr("AR100", user);
	IpAddress *ip = Identifiers::createIPv4("192.168.0.1", user);
	MacAddress *mac = Identifiers::createMac("aa:bb:cc:dd:ee:ff", user);

	int tmp1, tmp2;

	tmp1 = cntRi(pres, ar, mac, type);
	tmp2 = cntRi(pres, ar, mac, type, 1);
	
	if (tmp1 != 1 || tmp2 != 1)
		cerr << op << ": unexpected #RI for ar-mac link:" << tmp1  << " " << tmp2 << endl;

	tmp1 = cntRi(pres, ip, mac, type);
	tmp2 = cntRi(pres, ip, mac, type, 1);

	if (tmp1 != 1 || tmp2 != 1)
		cerr << op << ": unexpected #RI for ip-mac link:" << tmp1  << " " << tmp2 << endl;
	
	tmp1 = cntRi(pres, ip, type);
	tmp2 = cntRi(pres, ip, type, 1);

	if (tmp1 != 1 || tmp2 != 1)
		cerr << op << ": unexpected #RI for ip ident:" << tmp1  << " " << tmp2 << endl;

	delete ar;
	delete mac;
	delete ip;
}

static void
checkFirstPollResult(ARC *arc)
{
	PollResult *pres = arc->poll();

	if (cntUp(pres) != 0 || cntDe(pres) != 0 || cntNo(pres) != 0) {
		cerr << "second poll result contains non-deleteResults" << endl;
		return;
	}

	checkMetadataThere(pres, SEARCH,  "search");

	delete pres;
}

static void
checkDeletedSubgraph(ARC *arc)
{
	PollResult *pres = arc->poll();

	if (cntSe(pres) != 0 || cntUp(pres) != 0 || cntNo(pres) != 0) {
		cerr << "unexpected second poll result with non-delete results" << endl;
		return;
	}

	checkMetadataThere(pres, DELETE, "deleted");

	delete pres;
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
	PublishDelete *pd;
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
	pulist.push_back(Requests::createPublishUpdate(
				armac, ar->clone(), mac->clone()));

	pd = Requests::createPublishDelete(FILTER_MATCH_ALL, ar->clone(), mac);
	
	pr1 = Requests::createPublishReq(pulist);
	pr2 = Requests::createPublishReq(pd);
	pr1->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	
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
		checkDeletedSubgraph(arc);
		ssrc->endSession();
	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sr;
	delete pr1;
	delete pr2;
	delete arc;
	delete ssrc;
	return 0;
}
