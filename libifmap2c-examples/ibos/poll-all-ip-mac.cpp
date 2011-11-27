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
 * Check whether we can use the all-ip global identifier of IBOS
 */

#include <iostream>
#include <string>
#include <list>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/tcgifmapmeta.h>

#include "ibosglobalidentifier.h"

#include "common.h"

// make life easier
using namespace ifmap2c;
using namespace ifmap2cibos;
using namespace std;

#define GID_PREFIX "gid"
#define GID_HREF   "https://192.168.1.20/ifmap-schema/global-identifiers"
#define GID_NSPAIR STRP(GID_PREFIX, GID_HREF)

static void usage(const char *prog)
{
	cerr << "usage: " << prog << " "
		INDEPENDENT_USAGE_STRING << endl;
}

static void
outputSearchResult(SearchResult *const sr)
{
	list<ResultItem *> rilist = sr->getResultItems();
	list<ResultItem *>::const_iterator it, end;
	it = rilist.begin();
	end = rilist.end();
	ResultItem *ri;


	for (; it != end; it++) {
		ri = *it;
		// skip empty ResultItems as we only want to see
		// ip-mac
		if (ri->getMetadata().size() == 0)
			continue;

		// only interested in links, everything else is
		// bogus anyway
		if (!ri->getIdentifier1() || !ri->getIdentifier2())
			continue;

		if (!((typeid(*ri->getIdentifier1()) == typeid(IpAddress)
			&& typeid(*ri->getIdentifier2()) == typeid(MacAddress))
			||
			(typeid(*ri->getIdentifier2()) == typeid(IpAddress)
			&& typeid(*ri->getIdentifier2()) == typeid(MacAddress)))) {
			continue;
		}



		cout << "+--------------------------------------";
		cout << "---------------------------------------\n| ";
		cout << (*it)->getIdentifier1()->str() << " \t";
		cout << (*it)->getIdentifier2()->str() << "\t\t";
		cout << "#metadata=" << (*it)->getMetadata().size() << endl;
	}
	cout << "+--------------------------------------";
	cout << "---------------------------------------" << endl;
}

static void
outputPollResult(PollResult *const pr)
{
	list<SearchResult *>::const_iterator it, end;

	it = pr->getSearchResults().begin();
	end = pr->getSearchResults().end();

	if (it != end) {
		cout << "ip-mac in searchResults elements:" << endl;
		for (; it != end; it++)
			outputSearchResult(*it);
	}
	
	it = pr->getUpdateResults().begin();
	end = pr->getUpdateResults().end();

	if (it != end) {
		cout << "ip-mac in updateResult elements:" << endl;
		for (; it != end; it++)
			outputSearchResult(*it);
	}
	
	it = pr->getDeleteResults().begin();
	end = pr->getDeleteResults().end();

	if (it != end) {
		cout << "ip-mac in deleteResult elements:" << endl;
		for (; it != end; it++)
			outputSearchResult(*it);
	}
}

int main(int argc, char* argv[])
{
	char *url, *user, *pass, *capath;
	SSRC *ssrc = NULL;
	ARC *arc = NULL;
	SubscribeElement *su = NULL;
	SubscribeRequest *sreq = NULL;
	PollResult *pr = NULL;
	Identifier *allip = new AllIp();
	IdentifierHandlerDispatch::registerHandler(
			new AllIpHandler(GID_PREFIX, GID_HREF));

	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	su = Requests::createSubscribeUpdate(
			"all-ip-mac",
			FILTER_MATCH_ALL,
			2,
			"meta:ip-mac",
			SEARCH_NO_MAX_RESULT_SIZE,
			SEARCH_NO_TERMINAL_IDENTIFIERS,
			allip);

	sreq = Requests::createSubscribeReq(su);

	sreq->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	try {	
		ssrc->newSession();

		ssrc->subscribe(sreq);

		pr = arc->poll();
		cout << "Initial ip-mac links:" << endl;
		outputPollResult(pr);

		delete pr;

		while (true) {
			pr = arc->poll();
			cout << "Got pollResult:" << endl;
			outputPollResult(pr);
			delete pr;
			cout << endl;
		}

		// could use a signal handler or something to
		// do cleanup, but it's only for demonstration,
		// so it's ok...
		

		ssrc->endSession();

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sreq;
	delete ssrc;

	return 0;
}
