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
 * Check if there is no off-by one for subscription depth corner
 * case. Found in irond...
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
	PollResult *pres = arc->poll();
	SRLIST tmp1, tmp2, tmp3;

	tmp1 = pres->getUpdateResults();
	tmp2 = pres->getDeleteResults();
	tmp3 = pres->getNotifyResults();

	if (tmp1.size() != 0 || tmp2.size() != 0 || tmp3.size() != 0) {
		cerr << "unexpected second poll result with non-search results" << endl;
		goto clean;
	}

	if (pres->getSearchResults().size() != 1) {
		cerr << "unexpected initial poll result" << endl;
		goto clean;
	}

	sres = *pres->getSearchResults().begin();

	if (sres->getResultItems().size() != 3) {
		cerr << "unexpected number of resultItems: ";
		cerr << sres->getResultItems().size()  << endl;
	}

clean:
	delete pres;
}

static void
checkOnlyArCap(ARC *arc)
{
	PollResult *pres = arc->poll();
	SearchResult *sres;
	ResultItem *ri;
	AccessRequest *ar = Identifiers::createAr("abc");

	SRLIST updateResults;
	SRLIST tmp1, tmp2, tmp3;
	RILIST rilist;

	tmp1 = pres->getSearchResults();
	tmp2 = pres->getDeleteResults();
	tmp3 = pres->getNotifyResults();

	if (tmp1.size() != 0 || tmp2.size() != 0 || tmp3.size() != 0) {
		cerr << "unexpected second poll result with non-update results" << endl;
		goto clean;
	}

	updateResults = pres->getUpdateResults();

	if (updateResults.size() != 1) {
		cerr << "unexpected second poll result" << endl;
		goto clean;
	}


	sres = *updateResults.begin();

	rilist = sres->getResultItems();
	if (rilist.size() != 1) {
		cerr << "unexpected number of resultItems: ";
		cerr << rilist.size() << endl;
		goto clean;
	}


	rilist = sres->getResultItemsByType(ar);
	if (rilist.size() < 1) {
		cerr << "no resultItem for ar" << endl;
		goto clean;
	}

	ri = *rilist.begin();
	if (ri->getMetadata().size() != 1) {
		cerr << "unexpected number of metadata on ar identifier:";
		cerr << ri->getMetadata().size() << endl;
	}
clean:
	delete pres;
	delete ar;
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
	list<PublishElement *> pulist1, pulist2;
	SubscribeRequest *sr;
	SubscribeUpdate *su;
	Identifier *mac1, *mac2, *ar;
	XmlMarshalable *arcap, *armac, *ipmac;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	// Use user as administrative-domain to be able to run in
	// with multiple instances at once.
	ar = Identifiers::createAr("AR100", user);
	mac1 = Identifiers::createMac("aa:bb:cc:dd:ee:ff", user);
	mac2 = Identifiers::createMac("aa:bb:cc:dd:ee:f1", user);

	ipmac = Metadata::createIpMac();
	arcap = Metadata::createCapability("dummycap");
	armac = Metadata::createArMac();
	
	pulist1.push_back(Requests::createPublishUpdate(armac->clone(),
				ar->clone(), mac1->clone()));
	pulist1.push_back(Requests::createPublishUpdate(armac, ar->clone(), mac1->clone()));
	// This is cheating, but anyway ;)
	pulist2.push_back(Requests::createPublishUpdate(ipmac, mac1, mac2));
	pulist2.push_back(Requests::createPublishUpdate(arcap, ar->clone()));
	
	pr1 = Requests::createPublishReq(pulist1);
	pr2 = Requests::createPublishReq(pulist2);
	pr1->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	pr2->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	
	su = Requests::createSubscribeUpdate(
			"sub1",
			FILTER_MATCH_ALL,
			1,
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
		checkOnlyArCap(arc);
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
