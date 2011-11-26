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
 * Check if the max-depth for search works correctly, bugged in
 * irond 0.2.3
 */

#include <iostream>
#include <sstream>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"
#include "testcommon.h"

using namespace std;
using namespace ifmap2c;

static void
usage(const char *const name)
{
	cerr << "usage: " << name << INDEPENDENT_USAGE_STRING << endl;
}

#define ARCOUNT	4
#define for_ars(val)	for ((val) = 0; (val) < ARCOUNT ; (val)++)

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;

	SSRC *ssrc = NULL;
	PublishRequest *pr = NULL;
	SearchRequest *sr = NULL;
	SearchResult *sres = NULL;
	Identifier *ars[ARCOUNT];
	XmlMarshalable *md;
	list<PublishElement *> pulist;
	int i;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	ssrc = SSRC::createSSRC(url, user, pass, capath);

 	md = Metadata::createDevAttr("something");
	
	for_ars(i) {
		stringstream ss;
		ss << "AR" << i;
		ars[i] = Identifiers::createAr(ss.str(), user);
	}

	sr = Requests::createSearchReq(
			FILTER_MATCH_ALL,		/* match-links */
			2,				/* max-depth */
			FILTER_MATCH_ALL,		/* result-filter */
			SEARCH_NO_MAX_RESULT_SIZE,	/* max-size */
			ars[0]->clone());
	
	pulist.push_back(Requests::createPublishUpdate(md->clone(), ars[0]->clone(),
				ars[1]->clone()));
	pulist.push_back(Requests::createPublishUpdate(md->clone(), ars[1]->clone(),
				ars[2]->clone()));
	pulist.push_back(Requests::createPublishUpdate(md->clone(), ars[2]->clone(),
				ars[3]->clone()));
	pulist.push_back(Requests::createPublishUpdate(md, ars[0]->clone(), ars[2]->clone()));


	pr = Requests::createPublishReq(pulist);
	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	try {	
		ssrc->newSession();
		ssrc->publish(pr);
		sres = ssrc->search(sr);

		for_ars(i) {
			if (cntRi(sres, ars[i], 0) != 1)
				cerr << "AR" << i << " RI not found" << endl;
		}
		
		if (cntRi(sres, ars[0], ars[1], 1) != 1)
			cerr << "AR0-AR1 not in result" << endl;
		
		if (cntRi(sres, ars[0], ars[2], 1) != 1)
			cerr << "AR0-AR2 not in result" << endl;
		
		if (cntRi(sres, ars[1], ars[2], 1) != 1)
			cerr << "AR1-AR2 not in result" << endl;
		
		if (cntRi(sres, ars[2], ars[3], 1) != 1)
			cerr << "AR2-AR3 not in result" << endl;

		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}


	for_ars(i)
		delete ars[i];

	delete sr;
	delete sres;
	delete pr;
	delete ssrc;
	IdentifierHandlerDispatch::clearHandlers();
	IfmapRequestHandlerDispatch::clearHandlers();
	return 0;
}
