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
 * A simple test to check how MAPS return pollResults, you need to look at
 * the debug output to get something out of it, for else nothing is
 * special is done here.
 */


// libifmap2c includes
#include <libifmap2c/requests.h>
#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <cstdlib>
#include <list>


using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;

static STRP mdNs("metans", "http://mynamespace.com");

class SingleValueMetadata1 : public BasicXmlMarshalable {
public:
	SingleValueMetadata1(const string &content = "") :
		BasicXmlMarshalable("simpleMetadata1", content, mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};

class SingleValueMetadata2 : public BasicXmlMarshalable {
public:
	SingleValueMetadata2(const string &content = "") :
		BasicXmlMarshalable("simpleMetadata2", content, mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "singleValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};


static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 5)
		usage(argv[0]);

	// create ssrc object which is used for synchronous communication
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);
	ARC *arc = ssrc->getARC();

	// prepare a publish request to ip-address identifier with
	// metadata with two different types of singleValue metadata
	IpAddress *ip = Identifiers::createIPv4("192.168.0.1");

	list<SubPublish *> uplist;
	uplist.push_back(Requests::createPublishUpdate(
			new SingleValueMetadata1(),
			ip->clone()));
	uplist.push_back(Requests::createPublishUpdate(
				new SingleValueMetadata2(),
				ip->clone()));

	PublishRequest *pr1 = Requests::createPublishReq(uplist);


	// separate this.
	PublishRequest *pr2 = Requests::createPublishReq(
			Requests::createPublishUpdate(
				new SingleValueMetadata1(),
				ip->clone()));
	PublishRequest *pr3 = Requests::createPublishReq(
			Requests::createPublishUpdate(
				new SingleValueMetadata2(),
				ip->clone()));

	list<XmlMarshalable *> mlist;
	mlist.push_back(new SingleValueMetadata1());
	mlist.push_back(new SingleValueMetadata2());

	PublishRequest *pr4 = Requests::createPublishReq(
			Requests::createPublishUpdate(
				mlist, ip->clone()));


	SubSubscribe *subcreate = Requests::createSubscribeUpdate(
			"sub1",
			FILTER_MATCH_ALL,
			0,
			FILTER_MATCH_ALL,
			2000,
			ip);


	SubscribeRequest *subreq = Requests::createSubscribeReq(subcreate);

	try {
		cout << "Doing newSession\t";
		ssrc->newSession();
		cout << "Ok" << endl;

		cout << "Doing subscribe\t\t";
		ssrc->subscribe(subreq);
		cout << "Ok" << endl;

		cout << "Doing first poll\t";
		PollResult *pollres = arc->poll();
		delete pollres;
		cout << "Ok" << endl;

		sleep(2);
		cout << "Doing publish 1\t\t";
		ssrc->publish(pr1);
		cout << "Ok" << endl;

		cout << "Doing second poll\t";
		pollres = arc->poll();
		delete pollres;
		cout << "Ok" << endl;
		
		sleep(2);
		cout << "Doing publish 2\t\t";
		ssrc->publish(pr2);
		cout << "Ok" << endl;
		
		sleep(2);
		cout << "Doing publish 3\t\t";
		ssrc->publish(pr3);
		cout << "Ok" << endl;

		cout << "Doing fourth poll\t";
		pollres = arc->poll();
		delete pollres;
		cout << "Ok" << endl;
		
		sleep(2);
		cout << "Doing publish 4\t\t";
		ssrc->publish(pr4);
		cout << "Ok" << endl;

		cout << "Doing fifth poll\t";
		pollres = arc->poll();
		delete pollres;
		cout << "Ok" << endl;

		cout << "Doing endSession\t";
		ssrc->endSession();
		cout << "Ok" << endl;

	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "ErrorResult:" << endl;
		cerr << " " << e.getErrorCodeString() << endl;
		cerr << " " << e.getErrorString() << endl;
	} catch (XmlMarshalError e) {
		cerr << "XmlMarshalError: " << e.getMessage() << endl;
	} catch (ResponseParseError e) {
		cerr << "ResponseParseError: " << e.getMessage() << endl;
	} catch (...) {
		cerr << "Uncatched Exception occured" << endl;
		throw;
	}

	delete pr1;
	delete pr2;
	delete pr3;
	delete pr4;
	delete subreq;
	
	// this closes the TCP connections
	delete arc;
	delete ssrc;
	return 0;
}
