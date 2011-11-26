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
 * Check if we get an appropiate ErrorResult back in case we used too
 * much space on the MAPS.
 */

#include <iostream>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"

using namespace std;
using namespace ifmap2c;

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
	PublishRequest *pr = NULL;
	PublishUpdate *pu = NULL;
	SubscribeRequest *sr1, *sr2;
	SubscribeUpdate *su1, *su2;
	PollResult *pres = NULL;
	Identifier *ar1, *ar2;
	XmlMarshalable *md;
	bool ok = true;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	ar1 = Identifiers::createAr(user);
	ar2 = Identifiers::createAr(user, "secondOne");
	
	// 200 bytes
 	md = Metadata::createDevAttr(
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		);

	su1 = Requests::createSubscribeUpdate(
			"subscription1",
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_DEPTH,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			ar1->clone());
	su2 = Requests::createSubscribeUpdate(
			"subscription2",
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_DEPTH,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			ar2);
	
	sr1 = Requests::createSubscribeReq(su1);
	sr2 = Requests::createSubscribeReq(su2);

	pu = Requests::createPublishUpdate(md, ar1);
	pr = Requests::createPublishReq(pu);
	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);


	try {	
		ssrc->newSession(200);
		ssrc->subscribe(sr1);
		pres = arc->poll();
		delete pres;
		ssrc->publish(pr);
		try {
			pres = arc->poll();
			ok = false;
			delete pres;
		} catch (ErrorResult e){
			if (e.getErrorCode() == PollResultsTooBig)
				ok = true;
			else
				ok = false;
		}

		ssrc->subscribe(sr2);
		pres = arc->poll();

		if (pres->getSearchResults().size() != 1
				|| pres->getUpdateResults().size() != 0
				|| pres->getDeleteResults().size() != 0
				|| pres->getNotifyResults().size() != 0
				|| pres->getErrorResults().size() != 0) {
			cerr << "bad third poll result" << endl;
		} else {
			SearchResult *sres = pres->getSearchResults().front();
			if (sres->getName().compare("subscription2"))
				cerr << "bad subscription name" << endl;
		}
		delete pres;
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	if (!ok)
		cerr << "No PollResultsTooBig reply" << endl;

	delete sr1; delete sr2;
	delete pr;
	delete arc;
	delete ssrc;
	IdentifierHandlerDispatch::clearHandlers();
	IfmapRequestHandlerDispatch::clearHandlers();
	return 0;
}
