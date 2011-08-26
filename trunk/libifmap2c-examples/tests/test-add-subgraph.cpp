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

using namespace std;
using namespace ifmap2c;

typedef list<SearchResult *> SRLIST;
typedef SRLIST::iterator SRLISTIT;
typedef list<ResultItem *> RILIST;
typedef RILIST::iterator RILISTIT;

static void
checkFirstPollResult(ARC *arc)
{
	SearchResult *sres;
	ResultItem *ri;
	PollResult *pres = arc->poll();
	SRLIST tmp1, tmp2, tmp3;

	tmp1 = pres->getUpdateResults();
	tmp2 = pres->getDeleteResults();
	tmp3 = pres->getNotifyResults();

	if (tmp1.size() != 0 || tmp2.size() != 0 || tmp3.size() != 0) {
		cerr << "unexpected second poll result with non-update results" << endl;
		return;
	}

	if (pres->getSearchResults().size() != 1) {
		cerr << "unexpected initial poll result" << endl;
		return;
	}

	sres = *pres->getSearchResults().begin();

	if (sres->getResultItems().size() != 1) {
		cerr << "unexpected number of resultItems" << endl;
		return;
	}

	ri = *sres->getResultItems().begin();

	if (ri->getAccessRequest() == NULL) {
		cerr << "no access-request in first result" << endl;
		return;
	}

	if (ri->getMetadata().size() != 0) {
		cerr << "unexpected metadata count for resultItem" << endl;
		return;
	}

	delete pres;
}

static void
checkAddedSubgraph(ARC *arc)
{
	PollResult *pres = arc->poll();
	SearchResult *sres;
	ResultItem *ri;
	AccessRequest *ar = Identifiers::createAr("abc");
	IpAddress *ip = Identifiers::createIPv4("192.168.0.1");
	MacAddress *mac = Identifiers::createMac("aa:bb:cc:dd:ee:ff");

	bool ipDone, ipMacDone, arMacDone;
	ipDone =  ipMacDone = arMacDone = false;


	SRLIST updateResults;
	SRLIST tmp1, tmp2, tmp3;
	SRLISTIT srIt;
	SRLISTIT srEnd;
	RILIST rilist;

	tmp1 = pres->getSearchResults();
	tmp2 = pres->getDeleteResults();
	tmp3 = pres->getNotifyResults();

	if (tmp1.size() != 0 || tmp2.size() != 0 || tmp3.size() != 0) {
		cerr << "unexpected second poll result with non-update results" << endl;
		return;
	}

	updateResults = pres->getUpdateResults();

	if (updateResults.size() < 1) {
		cerr << "unexpected second poll result" << endl;
		return;
	}


	srIt = updateResults.begin();
	srEnd = updateResults.end();

	for ( ; srIt != srEnd; srIt++) {
		sres = *srIt;

		rilist = sres->getResultItemsByType(ar, mac);
		if (rilist.size() > 0) {
			if (rilist.size() != 1) {
				cerr << "unexpected resultItems for ar-mac link" << endl;
				return;
			}

			if (arMacDone) {
				cerr << "ar-mac link twice???" << endl;
				return;
			}


			ri = *rilist.begin();

			if (ri->getMetadata().size() != 1) {
				cerr << "unexpected metadata for ar-mac link" << endl;
				return;
			}
			
			arMacDone = true;
		}
		
		rilist = sres->getResultItemsByType(ip, mac);
		
		if (rilist.size() > 0) {
			if (rilist.size() != 1) {
				cerr << "unexpected resultItems for ip-mac link" << endl;
				return;
			}

			if (ipMacDone) {
				cerr << "ip-mac link twice???" << endl;
				return;
			}

			ri = *rilist.begin();

			if (ri->getMetadata().size() != 1) {
				cerr << "unexpected metadata for ip-mac link" << endl;
				return;
			}

			ipMacDone = true;
		}
		
		rilist = sres->getResultItemsByType(ip);
		
		if (rilist.size() > 0) {
			if (rilist.size() != 1) {
				cerr << "unexpected resultItems for ip identifier" << endl;
				return;
			}

			if (ipDone) {
				cerr << "ip twice???" << endl;
				return;
			}

			ri = *rilist.begin();

			if (ri->getMetadata().size() != 1) {
				cerr << "unexpected metadata ip link" << endl;
				return;
			}

			ipDone = true;
		}
	}
	
	if (!arMacDone)
		cerr << "Missing added metadata from ar-mac link" << endl;

	if (!ipMacDone)
		cerr << "Missing added metadata from ip-mac link" << endl;

	if (!ipDone)
		cerr << "Missing added metadata from ip identifier" << endl;

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
	char *url, *user, *pass, *capath;

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
	return 0;
}
