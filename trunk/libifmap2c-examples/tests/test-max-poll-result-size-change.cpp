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
 * Check if we can change the max-poll-result-size by doing a newSession
 * call.
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
	PublishRequest *pr;
	PublishUpdate *pu;
	SubscribeRequest *sr;
	SubscribeUpdate *su;
	PollResult *pres = NULL;
	Identifier *ar;
	XmlMarshalable *md;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	// create ssrc object which is used for synchronous communication
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	ar = Identifiers::createAr(user);
	
	// 240 bytes
 	md = Metadata::createDevAttr(
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		);
	md->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	su = Requests::createSubscribeUpdate(
			"subscription",
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_DEPTH,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			ar->clone());
	sr = Requests::createSubscribeReq(su);

	pu = Requests::createPublishUpdate(md, ar);
	pr = Requests::createPublishReq(pu);

	try {	
		ssrc->newSession(200);
		ssrc->subscribe(sr);
		pres = arc->poll();
		delete pres;
		ssrc->publish(pr);
		try {
			// should result in PollResultsTooBig
			pres = arc->poll();
			cerr << "no error" << endl;
			delete pres;
		} catch (ErrorResult e){
			if (e.getErrorCode() != PollResultsTooBig)
				cerr << "bad error" << endl;
		}

		// need to use a new one, otherwise irond isn't happy
		delete arc;
		arc = ssrc->getARC();

		// make sure we have a big enough max-poll-result-size
		ssrc->newSession(1000);
		ssrc->subscribe(sr);
		pres = arc->poll();
		delete pres;
		ssrc->publish(pr);
		
		// this should work, because there's enough space now
		pres = arc->poll();
		delete pres;
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
