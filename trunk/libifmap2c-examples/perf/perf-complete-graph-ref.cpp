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
createSubscriptions(SSRC *ssrc)
{
	ssrc->renewSession();
}

static void
publishMetaOnNodes(SSRC *ssrc)
{
	int i;
	for_all_idents(i)
		ssrc->renewSession();
}



static void
publishLink(SSRC *ssrc)
{
	ssrc->renewSession();
}

static void
publishCompleteGraph(SSRC *ssrc)
{
	int i, j;
	for_all_idents(i) {
		for_all_idents(j) {
			if (i == j)
				continue;

			publishLink(ssrc);
			ssrc->renewSession();
		}
	}
}

static void
checkFirstSearchResult(SSRC *ssrc)
{
	ssrc->renewSession();
}


int
main(int argc, char *argv[])
{
	SSRC *ssrc = NULL;
	char *url, *user, *pass, *capath;

	checkAndLoadParameters(argc, argv, 3, usage, &url, &user, &pass,
			&capath);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);

	graphSize = atoi(argv[1]);
	mdNode = atoi(argv[2]);
	mdLink = atoi(argv[3]);


	try {
		ssrc->newSession();
		createSubscriptions(ssrc);
		publishMetaOnNodes(ssrc);
		checkFirstSearchResult(ssrc);
		publishCompleteGraph(ssrc);
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}
	
	delete ssrc;
}
