/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2010-2011, Arne Welzel, <arne.welzel@googlemail.com>
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
 * Performance Measurement of a MAPS using a complete graph and some
 * subscriptions...
 */


// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>

#include "common.h"

using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;
typedef list<SearchResult *> SRLIST;
typedef list<ResultItem *> RILIST;
typedef list<XmlMarshalable *> XMLMLIST;

static unsigned int graphSize;
static int mdNode;
static int mdLink;

static void
usage(const char *name)
{
	cerr << "Usage: " << name << " <nodes> <metadata per node> "
		"<metadata per link> " INDEPENDENT_USAGE_STRING << endl;
}

#define for_all_idents(value)	for ((value) = 0; (unsigned int)(value) < graphSize; (value)++)

static void
createSubscriptions(Identifier **idents, SSRC *ssrc)
{
	int i;
	SubscribeRequest *sr;
	list<SubscribeElement *> list;

	for_all_idents(i) {
		stringstream ss;
		ss << "sub_ident_" << i;
		list.push_back(Requests::createSubscribeUpdate(
					ss.str(),
					"meta:role",
					graphSize - 1,
					"meta:role",
					SEARCH_NO_MAX_RESULT_SIZE,
					idents[i]->clone()));
	}

	sr = Requests::createSubscribeReq(list);
	sr->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	ssrc->subscribe(sr);
	delete sr;
}

static void
publishMetaOnNodes(Identifier **idents, SSRC *ssrc)
{
	int i;
	PublishRequest *pr;

	if (mdNode <= 0)
		return;
	
	for_all_idents(i) {
		XMLMLIST list;
		for (int j = 0; j < mdNode; j++) {
			stringstream ss;
			ss << "role_" << j << "_on_identifier_" << i;
			list.push_back(Metadata::createRole(ss.str()));
		}
		pr = Requests::createPublishReq(
				Requests::createPublishUpdate(list, idents[i]->clone()));
		pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);
		ssrc->publish(pr);
		delete pr;
	}
}



static void
publishLink(Identifier *i1, Identifier *i2, SSRC *ssrc)
{
	XMLMLIST list;
	PublishRequest *pr;
	XmlMarshalable *md = Metadata::createRole("some role");

	for (int i = 0; i < mdLink; i++)
		list.push_back(md->clone());

	pr = Requests::createPublishReq(Requests::createPublishUpdate(
				list, i1->clone(), i2->clone()));

	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	ssrc->publish(pr);
	delete pr;
	delete md;

}

static int
getCountOfMetadata(RILIST rilist)
{
	int ret = 0;
	RILIST::iterator start = rilist.begin();
	RILIST::iterator end = rilist.end();

	for ( ; start != end; start++)
		ret += (*start)->getMetadata().size();

	return ret;
}

static int
getCountOfMetadata(SRLIST list)
{
	SRLIST::iterator start = list.begin();
	SRLIST::iterator end = list.end();
	int ret = 0;
	
	for ( ; start != end; start++)
		ret += getCountOfMetadata((*start)->getResultItems());

	return ret;
}

static int
getCountOfMetadata(PollResult *pres)
{
	int ret = 0;
	if (pres->getErrorResults().size() > 0) {
		list<ErrorResult *>::const_iterator it = pres->getErrorResults().begin();
		list<ErrorResult *>::const_iterator end = pres->getErrorResults().end();
		for (/* */; it != end; it++) {
			cerr << **it << endl;
		}
	}
	ret += getCountOfMetadata(pres->getSearchResults());
	ret += getCountOfMetadata(pres->getUpdateResults());
	ret += getCountOfMetadata(pres->getDeleteResults());
	ret += getCountOfMetadata(pres->getNotifyResults());

	return ret;
}

static void
publishCompleteGraph(Identifier **idents, SSRC *ssrc, ARC *arc)
{
	PollResult *pres;
	int i, j, count, expected;
	int xxx = 0;
	for_all_idents(i) {
		for_all_idents(j) {
			if (j <= i)
				continue;
			cout << ".";
			cout.flush();

			publishLink(idents[i], idents[j], ssrc);
			pres = arc->poll();
			xxx += 2;
			
			count = getCountOfMetadata(pres);

			if (i == 0)
				expected = 2 * j * (mdNode + mdLink);
			else
				expected = graphSize * mdLink;

			/*
			cout << "i=" << i << " j=" << j;
			cout << " count=" << count;
			cout << " expected=" << expected << endl;
			*/

			if (count != expected) {
				cerr << "Unexpected metadata count ";
				cerr << "i=" << i << " j=" << j;
				cerr << " count=" << count;
				cerr << " expected=" << expected << endl;
				return;
			}


			delete pres;
		}
		//getchar();
		cout << endl;
	}
	cout << xxx << endl;
}

static void
checkFirstSearchResult(ARC *arc)
{
	PollResult *pres = arc->poll();
	int count;
	
	if (pres->getSearchResults().size() != graphSize)
		cerr << "Unexpected SearchResults" << endl;

	count = getCountOfMetadata(pres);
	
	if (pres->getUpdateResults().size() > 0)
		cout << "Note: MAPS Returns updateResults in first pollResult" << endl;
		
	if (count != mdNode * (signed int)graphSize) {
		cerr << "Unexpected SearchResults Metadata ";
		cerr << "expected= " << mdNode * (signed int)graphSize;
		cerr << "got=" << count << endl;
	}

	delete pres;
}


int
main(int argc, char *argv[])
{
	SSRC *ssrc = NULL;
	ARC *arc = NULL;
	char *url, *user, *pass, *capath;
	int i;
	Identifier **idents;

	checkAndLoadParameters(argc, argv, 3, usage, &url, &user, &pass,
			&capath);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	graphSize = atoi(argv[1]);
	mdNode = atoi(argv[2]);
	mdLink = atoi(argv[3]);

	idents = new Identifier* [graphSize];

	for_all_idents(i) {
		stringstream ss;
		ss << "user_" << i;
		idents[i] = Identifiers::createIdentity(username, ss.str(), user);
	}

	try {
		ssrc->newSession();
		createSubscriptions(idents, ssrc);
		publishMetaOnNodes(idents, ssrc);
		checkFirstSearchResult(arc);

		publishCompleteGraph(idents, ssrc, arc);
		//getchar();

		ssrc->endSession();
	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}
	
	for_all_idents(i)
		delete idents[i];
	
	delete idents;
	delete ssrc;
	delete arc;
}
