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

/* filename example-mapc2.cpp */

/*
 * A sample MAPC showing how to handle SearchResults and ResultItems,
 * respectively.
 *
 * The sanity checks might or might not be correct with some MAPSs
 * implementation. This was against irond 0.2.2.
 */

#include <iostream>
#include <list>

#include <libifmap2c/ssrc.h>
#include <libifmap2c/requests.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>


// libifmap2c namespace
using namespace ifmap2c;

using namespace std;

typedef ResultItem *RI;
typedef list<RI> RILIST;

// Example client for the Wiki, very extensive documented.
int main(int argc, char *argv[])
{
	if (argc != 5) {
		cout << "Usage: " << argv[0] << " ifmap-server-url"
			" user pass capath" << endl;
		return 1;
	}

	// Create a synchronous send receive channel using basic
	// authentication
	// the parameters are as follows:
	// ifmap-server-url, user, password, capath (path where the server
	// certificate is lying prepared with c_rehas)
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3], argv[4]);

	// create Identifier objects using the Identifiers class
	IpAddress *ip1 = Identifiers::createIPv4("192.168.0.1");
	MacAddress *mac1 = Identifiers::createMac("aa:bb:cc:dd:ee:ff");
	IpAddress *ip2 = Identifiers::createIPv4("192.168.0.2");
	MacAddress *mac2 = Identifiers::createMac("aa:bb:cc:dd:ee:01");
	AccessRequest *ar1 = Identifiers::createAr("ar01");
	AccessRequest *ar2 = Identifiers::createAr("ar02");
	Identity *id = Identifiers::createIdentity(username, "fool");

	// create a metadata objects, using the Metadata class
	XmlMarshalable *ipmac = Metadata::createIpMac();
	XmlMarshalable *armac = Metadata::createArMac();
	XmlMarshalable *authas = Metadata::createAuthAs();


	ipmac->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	armac->addXmlNamespaceDefinition(TCG_META_NSPAIR);
	authas->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	PublishUpdate *pubUpdate1 = Requests::createPublishUpdate(ipmac->clone(),
			ip1, session, mac1->clone());
	
	PublishUpdate *pubUpdate2 = Requests::createPublishUpdate(ipmac,
			ip2, session, mac2->clone());
	
	PublishUpdate *pubUpdate3 = Requests::createPublishUpdate(armac->clone(),
			mac1, session, ar1->clone());
	
	PublishUpdate *pubUpdate4 = Requests::createPublishUpdate(armac,
			mac2, session, ar2->clone());
	
	PublishUpdate *pubUpdate5 = Requests::createPublishUpdate(authas->clone(),
			id->clone(), session, ar1);
	
	PublishUpdate *pubUpdate6 = Requests::createPublishUpdate(authas,
			id->clone(), session, ar2);


	// create lists of PublishUpdate objects to be used
	// in PublishRequest objets
	list<PublishElement *> publishList1, publishList2, publishList3;
	publishList1.push_back(pubUpdate1);
	publishList1.push_back(pubUpdate2);
	publishList2.push_back(pubUpdate3);
	publishList2.push_back(pubUpdate4);
	publishList3.push_back(pubUpdate5);
	publishList3.push_back(pubUpdate6);

	// initialize the PublishRequests with the elements stored
	// in the lists
	PublishRequest *publishReq1 = Requests::createPublishReq(publishList1);
	PublishRequest *publishReq2 = Requests::createPublishReq(publishList2);
	PublishRequest *publishReq3 = Requests::createPublishReq(publishList3);


	// create a search request
	SearchRequest *sreq = Requests::createSearchReq(
			FILTER_MATCH_ALL,		// match-links-filter
			5,				// max-depth
			FILTER_MATCH_ALL,		// result-filter
			SEARCH_NO_MAX_RESULT_SIZE,	//max-size
			id);				// start identifier


	// run everything ;-)
	try {
		// run a newSession request
		cout << "newSession... ";
		ssrc->newSession();
		cout << "Ok!" << endl;;

		cout << "session-id=" << ssrc->getSessionId();
		cout << " ifmap-publisher-id=" << ssrc->getPublisherId() << endl;

		// run the publish request
		cout << "publish 1...  ";
		ssrc->publish(publishReq1);
		cout << "Ok!" << endl;
		
	
		cout << "search 1...   ";
		SearchResult *result1 = ssrc->search(sreq);

		// this result should only contain one result item
		if (result1->getResultItems().size() != 1)
			cout << "Got more result items than "
				"expected for first search result" << endl;

		// if we search for the identity, we should get the wanted
		// result item
		RILIST rlist = result1->getResultItemsByIdentifier(id);
		if (rlist.size() < 1) {
			cout << "The identity was not in the "
				"search result?!" << endl;
		} else {
			// get the first ResultItem of the list.
			RI idRI = rlist.front();
			// The identity identifier should not have any
			// metadata attached to it:
			if (idRI->getMetadataList().size() != 0) {
				cout << "Identity has Metadata "
					"attached to it?!" << endl;
			}
		}
		cout << "Ok!" << endl;

		cout << "publish 2...  ";
		ssrc->publish(publishReq2);
		cout << "Ok!" << endl;
		cout << "publish 3...  ";
		ssrc->publish(publishReq3);
		cout << "Ok!" << endl;


		cout << "search 2...   ";
		SearchResult *result2 = ssrc->search(sreq);

		// some sanity checks
		if (result2->getResultItems().size() != 13) {
			cout << "Got a different number of result "
				" items than expected for the second "
				"search" << endl;
		}

		// get the ip-mac link result item for ip1 and mac1
		rlist = result2->getResultItemsByIdentifier(ip1, mac1);
		if (rlist.size() < 1) {
			cout << "Link between ip1 and mac1 not in the "
				"search result?!" << endl;
		} else {
			// We only try the first element and hope
			// there is no other one...
			RI ipmacRI = rlist.front();
			if (ipmacRI->getMetadataList().size() != 1)
				cout << "Metadata not found?" << endl;

			list<XmlMarshalable *>mdlist =
				XmlMarshalable::findMatchingElements(
					ipmacRI->getMetadataList(),  // where to look 
					"ip-mac",  // element name, no nsprefix
					TCG_META_HREF);  // associated namespace

			if (mdlist.size() < 1) {
				cout << "ip-mac metadata not found!" << endl;
			}
		}

		// get all ResultItems having the type of the parameters given
		list<ResultItem *> ipmaclinks = result2->getResultItemsByType(ip1, mac1);
		list<ResultItem *> ars = result2->getResultItemsByType(ar1);

		if (ipmaclinks.size() != 2) {
			cout << "Wrong number of ipmaclinks" << endl;

		}
		if (ars.size() != 2) {
			cout << "Wrong number of ARs" << endl;
		}

		cout << "Ok!" << endl;

		// delete result1 and result2
		// be aware, any ResultItem which was taken from
		// these results will be deleted too!
		delete result1;
		delete result2;


		// run end session request
		cout << "endSession... ";
		ssrc->endSession();
		cout << "Ok!" << endl;;
	
	// If something goes wrong with the connection, marshalling
	// requests / responses, an IfmapError is thrown.
	} catch (IfmapError e) {
		cerr << e << endl;
	
	// ErrorResultError is thrown, if a errorResult is received from
	// the server. For example InvalidSessionID or InvalidMetadata.
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}

	delete publishReq1;
	delete publishReq2;
	delete publishReq3;
	delete sreq;

	// deleting the SSRC, this will close the TCP connection
	delete ssrc;

	return 0;
}
