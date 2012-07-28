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
 * Check if we get an appropiate PollResult, containing an ErrorResult
 * back in the case the results for a subscription a grow too big.
 * Further, check that a second, independent, subscription is still
 * functional after the first one exceeded its max-size.
 *
 * (THIS MIGHT BE MAPS DEPENDENT, IT WORKS FOR irond)
 */

#include <iostream>
#include <string>

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

#define BADSUB 	"exceedsub"
#define BADSUBMAX	200
#define GOODSUB 	"goodsub"
#define GOODSUBMAX	1000

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;

	SSRC *ssrc = NULL;
	ARC *arc = NULL;
	PublishRequest *pr = NULL;
	PublishUpdate *pu = NULL;
	SubscribeRequest *sr = NULL;
	SubscribeUpdate *su1 = NULL;
	SubscribeUpdate *su2 = NULL;
	list<SubscribeElement *> sulist;
	PollResult *pres = NULL;
	Identifier *ar;
	XmlMarshalable *md;
	string errsub;
	string goodsub;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	ar = Identifiers::createAr(user);
	
	// 200 bytes
 	md = Metadata::createDevAttr(
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		);

	su1 = Requests::createSubscribeUpdate(
			BADSUB,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_DEPTH,
			FILTER_MATCH_ALL,
			BADSUBMAX,
			ar->clone());

	su2 = Requests::createSubscribeUpdate(
			GOODSUB,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_DEPTH,
			FILTER_MATCH_ALL,
			GOODSUBMAX,
			ar->clone());
	
	sulist.push_back(su1);
	sulist.push_back(su2);
	
	sr = Requests::createSubscribeReq(sulist);

	pu = Requests::createPublishUpdate(md, ar);
	pr = Requests::createPublishReq(pu);
	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);


	try {	
		ssrc->newSession();
		ssrc->subscribe(sr);

		pres = arc->poll();
		if (pres->getSearchResults().size() != 2) {
			cerr << "Unexpected searchResults count" << endl;
			goto clean;
		}
		delete pres;

		ssrc->publish(pr);
		pres = arc->poll();
		if (pres->getErrorResults().size() != 1) {
			cerr << "Unexpected errorResuls count" << endl;
			goto clean;
		}

		if (pres->getUpdateResults().size() != 1) {
			cerr << "Unexpected updateResuls count" << endl;
			goto clean;
		}

		errsub = pres->getErrorResults().front()->getName();
		if (errsub.compare(BADSUB)) {
			cerr << BADSUB " is not the bad one" << endl;
			goto clean;
		}
		delete pres;

		// this part is to see whether GOODSUB is still
		// there...
		ssrc->publish(pr);
		pres = arc->poll();
		if (pres->getUpdateResults().size() != 1) {
			cerr << "Unexpected updateResuls count" << endl;
			goto clean;
		}
		
		goodsub = pres->getUpdateResults().front()->getName();
		if (goodsub.compare(GOODSUB)) {
			cerr << GOODSUB " is not the good one" << endl;
			goto clean;
		}

		if (pres->getErrorResults().size() != 0
			|| pres->getSearchResults().size() != 0
			|| pres->getDeleteResults().size() != 0) {
			cerr << "Unexpected final poll result" << endl;
		}
		delete pres;
clean:
		ssrc->endSession();
	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sr;
	delete pr;
	delete arc;
	delete ssrc;
	return 0;
}
