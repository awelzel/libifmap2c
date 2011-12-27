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

#include <iostream>

#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

using namespace std;
using namespace ifmap2c;

typedef list<SearchResult *> SRLIST;
typedef list<SearchResult *>::const_iterator CSRLISTIT;
typedef list<ResultItem *> RILIST;
typedef RILIST::const_iterator CRILISTIT;


static void examineResultItem(ResultItem *const resItem)
{
	cout << "  - ";
	if (resItem->getAccessRequest()) {
		cout << "AR   ";
	}

	if (resItem->getIpAddress()) {
		cout << "IP   ";
	}

	if (resItem->getDevice()) {
		cout << "Dev   ";
	}

	if (resItem->getMacAddress()) {
		cout << "Mac   ";
	}
	if (resItem->getIdentity()) {
		cout << "Identity";
	}

	cout << endl;

}
static void examinePollResult(PollResult *const pollRes)
{
	SearchResult *curSr = NULL;
	ResultItem *curRi = NULL;
	SRLIST list = pollRes->getSearchResults();
	CSRLISTIT it = list.begin();
	CSRLISTIT end = list.end();
	RILIST rList;
	CRILISTIT rIt, rEnd;


	cout << "SearchResults: " << list.size() << endl;
	for (curSr = *it; it != end; curSr = *(++it)) {
		cout << "Subscription: " << curSr->getName() << endl;
		rList = curSr->getResultItems();
		// cout << "We have " << rList.size() << " result items in here" << endl;
		rIt = rList.begin();
		rEnd = rList.end();
		for (curRi = *rIt; rIt != rEnd; rIt++) {
			examineResultItem(curRi);
		}
	}

	list = pollRes->getUpdateResults();
	it = list.begin();
	end = list.end();
	cout << "UpdateResults: " << list.size() << endl;
	for (curSr = *it; it != end; curSr = *(++it)) {
		cout << "Subscription: " << curSr->getName() << endl;
		rList = curSr->getResultItems();
		// cout << "We have " << rList.size() << " result items in here" << endl;
		rIt = rList.begin();
		rEnd = rList.end();
		for (curRi = *rIt; rIt != rEnd; rIt++) {
			examineResultItem(curRi);
		}
	}

	list = pollRes->getDeleteResults();
	it = list.begin();
	end = list.end();
	cout << "DeleteResults: " << list.size() << endl;
	for (curSr = *it; it != end; curSr = *(++it)) {
		cout << "Subscription: " << curSr->getName() << endl;
		rList = curSr->getResultItems();
		//cout << "We have " << rList.size() << " result items in here" << endl;
		rIt = rList.begin();
		rEnd = rList.end();
		for (curRi = *rIt; rIt != rEnd; rIt++) {
			examineResultItem(curRi);
		}
	}

	list = pollRes->getNotifyResults();
	it = list.begin();
	end = list.end();
	cout << "NotifyResults: " << list.size() << endl;
	for (curSr = *it; it != end; curSr = *(++it)) {
		cout << "Subscription: " << curSr->getName() << endl;
		rList = curSr->getResultItems();
		// cout << "We have " << rList.size() << " result items in here" << endl;
		rIt = rList.begin();
		rEnd = rList.end();
		for (curRi = *rIt; rIt != rEnd; rIt++) {
			examineResultItem(curRi);
		}
	}

}

int main(int argc, char *argv[])
{
	if (argc != 5) {
		cout << "Usage: " << argv[0] << " ifmap-server-url"
				" user pass capath" << endl;
		return 1;
	}

	/* communication objects */
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3], argv[4]);
	ifmap2c::ARC *arc = ssrc->getARC();

	Identifier *i1 = Identifiers::createAr("tnc:001");
	Identifier *i2 = Identifiers::createIdentity(username, "john");


	PollResult *pollRes = NULL;
	XmlMarshalable *ipmac1 = Metadata::createIpMac();
	XmlMarshalable *ipmac2 = Metadata::createIpMac();
	PublishUpdate *pu1 = Requests::createPublishUpdate(ipmac1, i1, session, i2);
	PublishUpdate *pu2 = Requests::createPublishUpdate(ipmac2, i1->clone(), session);
	PublishRequest *pr1 = Requests::createPublishReq(pu1);
	PublishRequest *pr2 = Requests::createPublishReq(pu2);

	// meta namespace isn't defined by default anymore
	pr1->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	pr2->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	SubscribeUpdate *sub = Requests::createSubscribeUpdate("mysub",
			FILTER_MATCH_ALL, 25, FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE, i1->clone());
	SubscribeRequest *subreq = Requests::createSubscribeReq(sub);
	SubscribeDelete *subdel = Requests::createSubscribeDelete("mysub");
	SubscribeRequest *subreqdel = Requests::createSubscribeReq(subdel);


	try {
		ssrc->newSession();

		ssrc->renewSession();

		ssrc->subscribe(subreq);

		ssrc->renewSession();

		pollRes = arc->poll();
		examinePollResult(pollRes);
		delete pollRes;

		ssrc->publish(pr1);

		pollRes = arc->poll();
		examinePollResult(pollRes);
		delete pollRes;

		ssrc->publish(pr2);

		pollRes = arc->poll();
		examinePollResult(pollRes);
		delete pollRes;

		ssrc->subscribe(subreqdel);
		ssrc->endSession();

	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete pr1;
	delete pr2;
	delete subreq;
	delete subreqdel;
	delete arc;
	delete ssrc;

	return 0;
}
