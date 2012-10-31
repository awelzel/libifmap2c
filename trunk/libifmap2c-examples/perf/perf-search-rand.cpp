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

/* Performance measurement of random metadata types */
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>
#include <time.h>

#include "common.h"
#include "testcommon.h"

using namespace std;
using namespace ifmap2c;

static int depth;
static int mdPerNode;
static int countMdTypes;
static int searchDepth;

static void
usage(const char *name)
{
	cerr << "Usage: " << name << " <#depth> <#searchdepth> "
		"<#mdnode> <#metadata_types>"
		INDEPENDENT_USAGE_STRING << endl;
}


typedef pair<string, string> STRP;
// something short
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
prepare_graph(SSRC *ssrc, XmlMarshalable *mds[], IpAddress *ips[], int d)
{
	int i = 0;
	PublishRequest *pr;
	PublishElement *up;
	list<PublishElement *> plist;

	/* prepare huge publish request */
	for (i = 0; i < d; i++) {
		list<XmlMarshalable *> mdList;

		for (int j = 0; j < mdPerNode; j++) {
			int randMd2 = random() % countMdTypes;
			mdList.push_back(mds[randMd2]->clone());
		}

		if (mdPerNode > 0) {
			// metadata for the identifier
			up = Requests::createPublishUpdate(mdList,
						ips[i]->clone());
			mdList.clear();
			plist.push_back(up);
		}
		// metadata on the link
		int randMd = random() % countMdTypes;
		up = Requests::createPublishUpdate(mds[randMd]->clone(),
						   ips[i]->clone(),
						   ips[i + 1]->clone());
		plist.push_back(up);
	}
	
	pr = Requests::createPublishReq(plist);
	plist.clear();
	ssrc->publish(pr);
	delete pr;
	pr = NULL;

	return;
}

static void
do_search(SSRC *ssrc, IpAddress *ips[])
{
	SearchRequest *sr;
	for (int i = 0; i < depth + 1; i += 2 * searchDepth) {

		sr = Requests::createSearchReq(
				FILTER_MATCH_ALL,
				searchDepth,
				FILTER_MATCH_ALL,
				SEARCH_NO_MAX_RESULT_SIZE,
				ips[i]->clone());

		SearchResult *res = ssrc->search(sr);

		/* DEBUG:
		list<ResultItem *> results = res->getResultItems();
		cout << "RESULT: " << results.size() << endl;

		list<ResultItem *>::iterator it = results.begin();
		list<ResultItem *>::iterator end = results.end();

		for (; it != end; it++) {
			if ((*it)->getIdentifier1())
				cout << (*it)->getIdentifier1()->str() << endl;
			if ((*it)->getIdentifier2())
				cout << (*it)->getIdentifier2()->str() << endl;
			cout << "---" << endl;
		}
		*/

		delete sr;
		delete res;
	}

}

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath, tmpBuf[64];
	checkAndLoadParameters(argc, argv, 4, usage, &url, &user, &pass,
			&capath);

	depth = atoi(argv[1]);
	searchDepth = atoi(argv[2]);
	mdPerNode = atoi(argv[3]);
	countMdTypes = atoi(argv[4]);

	if (depth > 65536 || depth < 1) {
		cerr << "Depth is limited to 65536." << endl;
		exit(1);
	}
	
	if (countMdTypes > 65536 || countMdTypes < 1) {
		cerr << "Metadata type count is limited to 65536." << endl;
		exit(1);
	}

	SSRC *ssrc = SSRC::createSSRC(url, user, pass, capath);

	IpAddress *ips[depth + 1];
	XmlMarshalable *mds[countMdTypes];

	/* prepare all IP addresses */
	for (int i = 0; i < depth + 1; i++) {
		
		snprintf(tmpBuf, sizeof(tmpBuf), "192.168.%d.%d",
				(i / 256) % 256, i % 256);
		ips[i] = Identifiers::createIPv4(tmpBuf, user);
	}
	
	/* prepare all metadata types addresses */
	for (int i = 0; i < countMdTypes; i++) {
		
		snprintf(tmpBuf, sizeof(tmpBuf), "t%d", i);
		mds[i] = new MyMetadata(tmpBuf);
	}

	srandom(time(NULL));

	try {
		ssrc->newSession();

		prepare_graph(ssrc, mds, ips, depth);

		do_search(ssrc, ips);

		ssrc->endSession();

	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}
	
	for (int i = 0; i < depth + 1; i++)
		delete ips[i];
	
	for (int i = 0; i < countMdTypes; i++)
		delete mds[i];

	delete ssrc;
}
