/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2010, Arne Welzel, <arne.welzel@googlemail.com>
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

#include "ssrc.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"
#include "metadata.h"

#include <iostream>
#include <cstdlib>
#include <list>

using namespace std;
using namespace ifmap2c;

class SingleValueMetadata : public BasicXmlMarshalable {
public:
	SingleValueMetadata() :
		BasicXmlMarshalable("simpleMetadata", "", NO_NSPAIR)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
	}
};

class MultiValueMetadata : public BasicXmlMarshalable {
public:
	MultiValueMetadata() :
		BasicXmlMarshalable("simpleMetadata", "", NO_NSPAIR)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
	}
};


static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

static void
outputSearchResult(SearchResult *sresult) {
	list<ResultItem *> list = sresult->getResultItems();
	RILISTIT it = list.begin();
	RILISTIT end = list.end();

	for (ResultItem *item = *it; it != end; ++it, item = *it) {
		if (item->getAccessRequest()) {
			XmlMarshalable::putXmlMarshalable(item->getAccessRequest());
		}

		if (item->getDevice()) {
			XmlMarshalable::putXmlMarshalable(item->getDevice());
		}

		if (item->getIdentity()) {
			XmlMarshalable::putXmlMarshalable(item->getIdentity());
		}

		if (item->getIpAddress()) {
			XmlMarshalable::putXmlMarshalable(item->getIpAddress());
		}

		if (item->getMacAddress()) {
			XmlMarshalable::putXmlMarshalable(item->getMacAddress());
		}

		cerr << "METADATALIST" << endl;
		XMLMLIST mlist = item->getMetadataList();
		XMLMLISTIT mit = mlist.begin();
		XMLMLISTIT mend = mlist.end();

		for (XmlMarshalable *md = *mit; mit != mend; ++mit, md = *mit) {
			XmlMarshalable::putXmlMarshalable(md);
		}
	}

}

int
main(int argc, char *argv[])
{
	if (argc != 5)
		usage(argv[0]);

	// create ssrc object which is used for synchronous communication
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);
	XmlMarshalable *single = new SingleValueMetadata();
	XmlMarshalable *multi = new MultiValueMetadata();
	IpAddress *i1 = IpAddress::createIpv4Address("192.168.0.1");

	SubPublish *sub1 = PublishUpdate::createPublishUpdate(single, i1->clone(), session);
	SubPublish *sub2 = PublishUpdate::createPublishUpdate(multi, i1->clone(), session);
	PublishRequest *pr1 = PublishRequest::createPublishRequest(sub1);
	PublishRequest *pr2 = PublishRequest::createPublishRequest(sub2);

	SearchRequest *sr1 = SearchRequest::createSearchRequest(FILTER_MATCH_NOTHING,
			SEARCH_NO_MAX_DEPTH, FILTER_MATCH_ALL, SEARCH_NO_MAX_RESULT_SIZE, i1);
	SearchResult *sresult;


	try {
		cout << "Doing newSession\t";
		ssrc->newSession();
		cout << "Ok" << endl;
		cout << "Doing publish1\t\t";
		ssrc->publish(pr1);
		cout << "Ok" << endl;
		cout << "Doing publish2\t\t";
		try {
			ssrc->publish(pr2);
			cout << "Should have failed..." << endl;
		} catch (ErrorResultError e) {
			cout << "Ok Failed on purpose..." << endl;;
		}

		cout << "Doing search\t\t";
		sresult = ssrc->search(sr1);
		cout << "Ok" << endl;
		outputSearchResult(sresult);
		delete sresult;

		cout << "Doing purge\t\t";
		ssrc->purgePublisher();
		cout << "Ok" << endl;

		cout << "Doing search2\t\t";
		sresult = ssrc->search(sr1);
		cout << "Ok" << endl;
		outputSearchResult(sresult);
		delete sresult;


		cout << "Doing publish2\t\t";
		ssrc->publish(pr2);
		cout << "Ok" << endl;
		cout << "Doing publish1\t\t";
		try {
			ssrc->publish(pr1);
			cout << "Should have failed..." << endl;
		} catch (Resp) {
			cout << "Ok Failed on purpose..." << endl;;
		}

		cout << "Doing search3\t\t";
		sresult = ssrc->search(sr1);
		cout << "Ok" << endl;
		outputSearchResult(sresult);
		delete sresult;

		cout << "Doing publish2 again\t";
		ssrc->publish(pr2);
		cout << "Ok" << endl;

		cout << "Doing search4\t\t";
		sresult = ssrc->search(sr1);
		cout << "Ok" << endl;
		outputSearchResult(sresult);
		delete sresult;

		cout << "Doing endSession\t";
		ssrc->endSession();
		cout << "Ok" << endl;

	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "ErrorResult:" << endl;
		cerr << " " << e.getErrorCodeString() << endl;
		cerr << " " << e.getErrorString() << endl;
	} catch (...) {
		cerr << "Uncatched Exception occured" << endl;
		throw;
	}

	delete pr1;
	delete pr2;
	delete sr1;
	delete ssrc;
	return 0;
}

