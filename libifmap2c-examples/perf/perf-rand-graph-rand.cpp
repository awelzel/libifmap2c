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
#include "testcommon.h"

using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;
typedef list<SearchResult *> SRLIST;
typedef list<ResultItem *> RILIST;
typedef list<XmlMarshalable *> XMLMLIST;

static int nodes;
static int percentage;
static int ops;
static int possLinks;
static int opsCount;

static SSRC *ssrc;
static ARC *arc;
static char *url, *user, *pass, *capath;

static Identifier **idents;

#define for_all_idents(value)	for ((value) = 0; int(value) < nodes; (value)++)

static void
usage(const char *name)
{
	cerr << "Usage: " << name << " <nodes> <ops per possible link> <percentage del>"
		" <#metadata_types>"
		INDEPENDENT_USAGE_STRING << endl;
}

static STRP mdNs("n", "h:/xzx");

/* A metadata class with variable element name and cardinality multiValue */
class MyMetadata : public BasicXmlMarshalable {
public:
	MyMetadata(const string &elName) :
		BasicXmlMarshalable(elName, "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};


	
	
static void
createSubscriptions()
{
	int i;
	SubscribeRequest *sr;
	list<SubscribeElement *> list;

	for_all_idents(i) {
		stringstream ss;
		ss << "sub_ident_" << i;
		list.push_back(Requests::createSubscribeUpdate(
					ss.str(),
					FILTER_MATCH_ALL,
					nodes - 1,
					FILTER_MATCH_ALL,
					SEARCH_NO_MAX_RESULT_SIZE,
					idents[i]->clone()));
	}

	sr = Requests::createSubscribeReq(list);
	ssrc->subscribe(sr);
	delete sr;
}

int
main(int argc, char *argv[])
{
	int i, j, k, mdIdx;
	char tmpBuf[64];
	PollResult *pres;


	checkAndLoadParameters(argc, argv, 4, usage, &url, &user, &pass,
			&capath);
	
	nodes = atoi(argv[1]);
	ops = atoi(argv[2]);
	percentage = atoi(argv[3]);
	int countMdTypes = atoi(argv[4]);
	possLinks = (nodes * (nodes - 1)) / 2;
	opsCount = ops * possLinks;
	
	if (countMdTypes > 65536 || countMdTypes < 0) {
		cerr << "Metadata type count is limited to 65536." << endl;
		exit(1);
	}
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	idents = new Identifier* [nodes];
	
	XmlMarshalable *mds[countMdTypes];
	
	/* prepare all metadata types addresses */
	for (i = 0; i < countMdTypes; i++) {
		
		snprintf(tmpBuf, sizeof(tmpBuf), "t%d", i);
		mds[i] = new MyMetadata(tmpBuf);
	}

	for_all_idents(i) {
		stringstream ss;
		ss << "AR" << i;
		idents[i] = Identifiers::createAr(ss.str(), user);
	}


	// Initialize pseudo random number generator with number of nodes
	srand(nodes);



	try {
		ssrc->newSession();
		createSubscriptions();

		// Initial poll result.
		pres = arc->poll();
		checkContainsOnly(pres, SEARCH, "search result", nodes);
		checkRiCnt(pres, SEARCH, "search result", nodes);

		delete pres;
		for (int i = 0; i < opsCount; i++) {
			PublishRequest *pr;
			PublishElement *pe;
			Identifier *i1;
			Identifier *i2;
			bool doPoll = false;
			j = rand() % nodes;
			k = rand() % nodes;
			mdIdx = rand() % countMdTypes;
			
			if (j == k)
				k = (k + 1) % nodes;

			i1 = idents[j];
			i2 = idents[k];


			if ((rand() % 100) < percentage) {
				// do delete
				pe = Requests::createPublishDelete(FILTER_MATCH_ALL,
								   i1->clone(), i2->clone());
				cout << "-";
			} else {
				// do update
				pe = Requests::createPublishUpdate(mds[mdIdx]->clone(),
							i1->clone(), i2->clone());
				doPoll = true;
				cout << "+";
			}

			pr = Requests::createPublishReq(pe);
			ssrc->publish(pr);
			
			if (doPoll) {
				pres = arc->poll();
				delete pres;
			}

			cout.flush();
		}

		cout << endl;

		ssrc->endSession();

	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	for_all_idents(i)
		delete idents[i];
	
	for (i = 0; i < countMdTypes; i++)
		delete mds[i];

	delete[] idents;

	delete ssrc;
	delete arc;
}
