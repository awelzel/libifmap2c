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


/* test-subgraph1.cpp */

#include <iostream>
#include <string>
#include <sstream>
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
	char *url, *user, *pass, *capath;

	Identifier *ars[6];
	int i;
	PublishRequest *pr1, *pr2, *pr3;
	PublishDelete *pd;
	list<PublishElement *> pulist;
	SubscribeRequest *sr;
	SubscribeUpdate *su;
	XmlMarshalable *ipmac;
	PollResult *pres;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	for (i = 0; i < 6; i++) {
		stringstream ss;
		ss << "AR" << i;
		ars[i] = Identifiers::createAr(ss.str(), user);
	}

	ipmac = Metadata::createIpMac();
	
	pulist.push_back(Requests::createPublishUpdate(ipmac->clone(),
				ars[0]->clone(), ars[1]->clone()));
	pulist.push_back(Requests::createPublishUpdate(ipmac->clone(),
				ars[0]->clone(), ars[2]->clone()));

	pulist.push_back(Requests::createPublishUpdate(ipmac->clone(),
				ars[1]->clone(), ars[3]->clone()));
	pulist.push_back(Requests::createPublishUpdate(ipmac->clone(),
				ars[2]->clone(), ars[4]->clone()));
	
	pulist.push_back(Requests::createPublishUpdate(ipmac->clone(),
				ars[3]->clone(), ars[5]->clone()));
	pulist.push_back(Requests::createPublishUpdate(ipmac->clone(),
				ars[4]->clone(), ars[5]->clone()));


	pd = Requests::createPublishDelete(FILTER_MATCH_ALL,
			ars[2]->clone(), ars[4]->clone());
	
	pr1 = Requests::createPublishReq(pulist);
	pr2 = Requests::createPublishReq(pd);
	pr3 = Requests::createPublishReq(Requests::createPublishUpdate(ipmac->clone(),
				ars[2]->clone(), ars[4]->clone()));

	pr1->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	pr3->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	
	su = Requests::createSubscribeUpdate(
			"sub1",
			FILTER_MATCH_ALL,
			10,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			ars[0]->clone());
	
	sr = Requests::createSubscribeReq(su);

	try {	
		ssrc->newSession();
		ssrc->publish(pr1);
		ssrc->subscribe(sr);
		
		pres = arc->poll();

		if (cntUp(pres) != 0 || cntDe(pres) != 0 || cntNo(pres) != 0)
			cerr << "first poll result with non-search result" << endl;

		if (cntSe(pres) != 1)
			cerr << "unexpected initial poll result" << endl;

		for (i = 0; i < 6; i++)
			if (cntRi(pres, ars[i], SEARCH) != 1)
				cerr << "unexpected RI# for AR" << i << endl;
		
		if (cntRi(pres, ars[0], ars[1], SEARCH) != 1)
				cerr << "unexpected LINK 0 1" << endl;

		if (cntRi(pres, ars[0], ars[2], SEARCH) != 1)
				cerr << "unexpected LINK 0 2" << endl;

		if (cntRi(pres, ars[1], ars[3], SEARCH) != 1)
				cerr << "unexpected LINK 1 3" << endl;

		if (cntRi(pres, ars[2], ars[4], SEARCH) != 1)
				cerr << "unexpected LINK 2 4" << endl;
		delete pres;

		ssrc->publish(pr2);

		pres = arc->poll();
		
		if (cntUp(pres) != 0 || cntSe(pres) != 0 || cntNo(pres) != 0)
			cerr << "second poll result with non-delete result" << endl;

		if (cntRi(pres, DELETE) != 1)
			cerr << "unexpected RI# for delete-result" << endl;

		if (cntRi(pres, ars[2], ars[4], DELETE, 1) != 1)
			cerr << "LINK 2 4 not deleted?" << endl;

		delete pres;
		
		ssrc->publish(pr3);

		pres = arc->poll();
		if (cntDe(pres) != 0 || cntSe(pres) != 0 || cntNo(pres) != 0)
			cerr << "third poll result with non-update result" << endl;

		if (cntRi(pres, UPDATE) != 1)
			cerr << "unexpected RI# for update-result" << endl;

		if (cntRi(pres, ars[2], ars[4], UPDATE, 1) != 1)
			cerr << "LINK 2 4 not added?" << endl;
		delete pres;
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sr;
	delete pr1;
	delete pr2;
	delete pr3;
	delete arc;
	delete ssrc;
	return 0;
}
