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

/* Performance Measurement of a MAPS using a pulsing star graph */
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

static int armLength;
static int armCount;
static int pulseCount;
static char *url, *user, *pass, *capath;

static Identifier *root;
static XmlMarshalable *ipmac;

static void
usage(const char *name)
{
	cerr << "Usage: " << name << " <arms> <max length> <pulses>"
		INDEPENDENT_USAGE_STRING << endl;
}

#define forEachPulse(val) for((val) = 0; (val) < pulseCount; (val)++)
#define forEachArm(val) for((val) = 0; (val) < armCount; (val)++)
#define forEachLength(val) for((val) = 0; (val) < armLength; (val)++)

static void
addArmPart(SSRC *ssrc, int step)
{
	int i;
	list<PublishElement *> plist;
	PublishRequest *pr;

	forEachArm(i) {
		stringstream ssLast, ssNext;
		ssLast << "AR" << step - 1 << "-" << i;
		ssNext << "AR" << step << "-" << i;
		Identifier *last = Identifiers::createAr(ssLast.str(), user);
		Identifier *next = Identifiers::createAr(ssNext.str(), user);
		
		if (step != 0) {
			plist.push_back(Requests::createPublishUpdate(ipmac->clone(),
						last->clone(), next->clone()));
		} else {
			plist.push_back(Requests::createPublishUpdate(ipmac->clone(),
						root->clone(), next->clone()));
		}
		delete last;
		delete next;

	}
	pr = Requests::createPublishReq(plist);
	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	ssrc->publish(pr);
	delete pr;
}

static void
checkAddedArmPart(ARC *arc) {
	PollResult *pres = arc->poll();

	checkRiCnt(pres, UPDATE, "grow update result", armCount);

	delete pres;
}

static void
deleteArmPart(SSRC *ssrc, int step) {
	list<PublishElement *> plist;
	int i, j = armLength - step - 1;
	PublishRequest *pr;
	
	forEachArm(i) {
		stringstream ssLast, ssNext;
		ssLast << "AR" << j << "-" << i;
		ssNext << "AR" << j - 1 << "-" << i;
		
		Identifier *last = Identifiers::createAr(ssLast.str(), user);
		Identifier *next = Identifiers::createAr(ssNext.str(), user);
		
		if (j != 0) {
			plist.push_back(Requests::createPublishDelete(
						FILTER_MATCH_ALL,
						last->clone(), next->clone()));
		} else {
			plist.push_back(Requests::createPublishDelete(
						FILTER_MATCH_ALL,
						root->clone(), last->clone()));
		}
		delete last;
		delete next;

	}
	pr = Requests::createPublishReq(plist);
	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	ssrc->publish(pr);
	delete pr;
}

static void
checkDeletedArmPart(ARC *arc) {
	PollResult *pres = arc->poll();
	checkRiCnt(pres, DELETE, "shrink delete result", armCount);
	delete pres;
}

int
main(int argc, char *argv[])
{
	SSRC *ssrc = NULL;
	ARC *arc = NULL;
	int i, j;
	list<PublishElement *> plist;
	SubscribeRequest *sr;
	PollResult *pres;

	checkAndLoadParameters(argc, argv, 3, usage, &url, &user, &pass,
			&capath);
	
	armCount = atoi(argv[1]);
	armLength = atoi(argv[2]);
	pulseCount = atoi(argv[3]);
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	ipmac = Metadata::createIpMac();
	root = Identifiers::createAr("ROOTAR", user);
	sr = Requests::createSubscribeReq(
			Requests::createSubscribeUpdate(
					"sub",
					"meta:ip-mac",
					armLength,
					"meta:ip-mac",
					SEARCH_NO_MAX_RESULT_SIZE,
					root->clone()));

	sr->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	try {
		ssrc->newSession();
		ssrc->subscribe(sr);

		pres = arc->poll();
		checkContainsOnly(pres, SEARCH, "search result", 1);
		checkRiCnt(pres, SEARCH, "search result", 1);

		delete pres;

		forEachPulse(j) {
			// grow the star
			forEachLength(i) {
				addArmPart(ssrc, i);
				checkAddedArmPart(arc);
				cout << "+";
				cout.flush();
			}

			// shrink the star
			forEachLength(i) {
				deleteArmPart(ssrc, i);
				checkDeletedArmPart(arc);
				cout << "-";
				cout.flush();
			}
			cout << endl;
		}

		ssrc->endSession();

	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sr;
	delete ipmac;
	delete root;
	delete ssrc;
	delete arc;
}
