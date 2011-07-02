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
 * Check whether we receive notify metadata correctly, and no metadata
 * we are not interested in.
 *
 * This might give MAPS specific results, depending on whether the
 * MAPS really supports namespaces and how PollResults are constructed.
 * There is also some "problem" that synchronization between SSRC and ARC
 * is soley based on the usleep(SLEEPTIME) calls, which isn't the most
 * elegant way to do it...
 */


// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <list>

extern "C" {
	#include <pthread.h>
}

#include "common.h"

#define SLEEPTIME 50000

using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;
typedef list<SearchResult *> SRLIST;
typedef list<ResultItem *> RILIST;
typedef list<XmlMarshalable *> XMLMLIST;

static void
usage(const char *name)
{
	cerr << "Usage: " << name << INDEPENDENT_USAGE_STRING << endl;
}

static void *
pollThreadFunc(void *arg)
{
	ARC *arc = (ARC *)arg;
	int expect_sres, expect_nres, expect_end;
	int mdcount = 0;
	PollResult *pres;
	SRLIST rlist;
	RILIST rilist;
	
	expect_sres = 1;
	expect_nres = expect_end = 0;

	while (true){
		try {
			pres = arc->poll();
			
			if (expect_sres) {
				rlist = pres->getSearchResults();
				expect_sres = 0;
				expect_nres = 1;
			} else if (expect_nres) {
				rlist = pres->getNotifyResults();
			} else {
				cerr << "[poll] bad control flow" << endl;
				delete pres;
				break;
			}

			if (rlist.size() != 1) {
				cerr << "[poll] bad results" << endl;
				delete pres;
				continue;
			}

			rilist = rlist.front()->getResultItems();
			if (rilist.size() != 1) {
				cerr << "[poll] bad resultitems" << endl;
				delete pres;
				continue;
			}
			mdcount += rilist.front()->getMetadata().size();
			delete pres;
			if (mdcount >= 2) {
				expect_sres = 0;
				expect_nres = 0;
				expect_end = 1;
			}

		} catch (EndSessionResult e) {
			if (!expect_end || mdcount != 2) {
				cerr << "[poll] bad ending" << endl;
				cerr << "[poll] mdcount=" << mdcount;
				cerr << endl;
				return (void*)1;
			}
			return (void*)0;
		} catch (IfmapError e) {
			cerr << "[poll] " << e << endl;
		} catch (ErrorResultError e) {
			cerr << "[poll] " << e << endl;
		}
	}
	cerr << "[poll] bad control flow" << endl;
	return (void*)1;
}

int
main(int argc, char *argv[])
{
	SSRC *ssrc;
	ARC *arc;
	char *url, *user, *pass, *capath;
	XmlMarshalable *md1, *md2, *md3;
	XMLMLIST mlist;
	PublishRequest *pr1, *pr2;
	SubscribeRequest *sr;
	Identifier *ip;
	pthread_t pollThread;
	void *tRet = 0;
	int ret;

	checkAndLoadParameters(argc, argv, 0, usage, &url, &user, &pass,
			&capath);

	ssrc = SSRC::createSSRC(url, user, pass, capath);
	arc = ssrc->getARC();

	md1 = new UserMetadata("simpl", "n1", "http://testnotify");
	md1->addXmlNamespaceDefinition(STRP("n1", "http://testnotify"));

	md2 = new UserMetadata("simpl", "n2", "http://testnotify");
	md2->addXmlNamespaceDefinition(STRP("n2", "http://testnotify"));
	
	md3 = new UserMetadata("simpl", "n1", "http://unwanted");
	md3->addXmlNamespaceDefinition(STRP("n1", "http://unwanted"));

	ip = IpAddress::createIpv4Address("192.168.0.1");

	mlist.push_back(md2);
	mlist.push_back(md3);
	pr1 = Requests::createPublishReq(
			Requests::createPublishNotify(md1, ip->clone()));
	pr2 = Requests::createPublishReq(
			Requests::createPublishNotify(mlist, ip->clone()));
	sr = Requests::createSubscribeReq(
			Requests::createSubscribeUpdate(
				"sub1",
				FILTER_MATCH_NOTHING,
				SEARCH_NO_MAX_DEPTH,
				"ns:simpl",	/* result filter */
				SEARCH_NO_MAX_RESULT_SIZE,
				ip));
	sr->addXmlNamespaceDefinition(STRP("ns", "http://testnotify"));

	try {
		ssrc->newSession();
		ret = pthread_create(&pollThread, NULL, pollThreadFunc,
				(void*)arc);
		if (ret) {
			cerr << "[ERROR] starting thread" << endl;
			goto clean;
		}
		ssrc->subscribe(sr);
		usleep(SLEEPTIME);
		ssrc->publish(pr1);
		usleep(SLEEPTIME);
		ssrc->publish(pr2);
		usleep(SLEEPTIME);
		ssrc->endSession();
		pthread_join(pollThread, &tRet);

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}

	if (tRet)
		cerr << "Bad metadata count received" << endl;
clean:
	delete pr1; delete pr2; delete sr;
	delete arc;
	delete ssrc;
	return 0;
}
