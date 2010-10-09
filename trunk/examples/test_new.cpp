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

#include <cstdio>
#include <iostream>
#include <string>
#include "xmlmarshalable.h"
#include "xmlmarshaller.h"
#include "xmlunmarshaller.h"
#include "basicxmlmarshalable.h"
#include "ifmapcommunication.h"
#include "lowlevelcommunication.h"
#include "lowlevelcurlcommunication.h"
#include "libxml2marshaller.h"
#include "libxml2unmarshaller.h"
#include "identifiers.h"
#include "requests.h"
#include "communicationerror.h"
#include "errorresulterror.h"
#include "typedefs.h"
#include "metadatafactory.h"
#include "responseparseerror.h"

using namespace std;

int main(int argc, char *argv[])
{
	// basic authentication
	IfmapCommunication c(
			"https://localhost:8443", "test", "test",
			"cadir");


	// credential authentication
	// IfmapCommunication c(
	//		"https://localhost:8444", "./mykey/mapc.pem",
	//		"mapclient", "./mykey/mapc.crt", "cadir");

	NewSessionResult *nsresult = NULL;;

//	AccessRequest *ar1 = AccessRequest::createAccessRequest("blub");
//	AccessRequest *ar2 = AccessRequest::createAccessRequest("blub2", "mydomain");
//	Device *dev1 = Device::createDeviceName("devicedevice");
//	Device *dev2 = Device::createDeviceAik("aikaikname");
	IpAddress *ip1 = IpAddress::createIpv4Address("192.168.0.1", "mydomain");
//	IpAddress *ip2 = IpAddress::createIpv4Address("192.168.0.1");
//	IpAddress *ip3 = IpAddress::createIpv6Address("aa::00:11:xy");
//	IpAddress *ip4 = IpAddress::createIpv6Address("aa::00:11:xy", "mydomain2");
	MacAddress *mac1 = MacAddress::createMacAddress("00:aa:bb:cc:ee:11");
//	MacAddress *mac2 = MacAddress::createMacAddress("00:aa:bb:cc:ee:11", "mydomain");

	SearchRequest *sr1 = Requests::createSR(NULL, 5, NULL, -1, ip1);
	SearchRequest *sr2 = Requests::createSR(NULL, 5, NULL, -1, ip1);

	sr1->addXmlNamespaceDefinition(META_NSPAIR);

	string sId;

	XmlMarshalable *ipMac = Metadata::createIpMac();
	XmlMarshalable *ipMac2 = Metadata::createIpMac("00201002", "20021", "theServer");

	PublishUpdate *updateIpMac = Requests::createPU(ipMac, mac1, session, ip1);
	PublishUpdate *updateIpMac2 = Requests::createPU(ipMac2, mac1, session, ip1);
	PublishRequest *pr1 = Requests::createPR(updateIpMac);
	PublishRequest *pr2 = Requests::createPR(updateIpMac2);


	PublishDelete *del = Requests::createPD(NULL, mac1, ip1);
	PublishRequest *pr3 = Requests::createPR(del);




	try {
		cout << "Doing newSession... ";
		nsresult = c.newSession("1234");
		cout << "Ok!" << endl;
		sId = nsresult->getSessionId();
		cout << "My SessionId = " << sId << endl;
		cout << "My PublisherId = " << nsresult->getPublisherId() << endl;

		/* just publish random data */
		cout << "Doing publish1... ";
		c.publish(pr1);
		cout << "Ok! " << endl;

		cout << "Doing publish2... ";
		c.publish(pr2);
		cout << "Ok! " << endl;



		cout << "Doing search... " << endl;
		SearchResult *sr = c.search(sr1);
		cout << "Got " << sr->getResultItems().size() << " ResultItems." << endl;
		delete sr;

		cout << "Doing publishDelete... " << endl;
		c.publish(pr3);
		cout << "Ok!" << endl;

		cout << "Doing search again... " << endl;
		sr = c.search(sr2);
		cout << "Got " << sr->getResultItems().size() << " ResultItems." << endl;


		cout << "Doing endSession... ";
		c.endSession();
		cout << "Ok!" << endl;
		cout << "Doing endSession... ";
		c.endSession();

	} catch (CommunicationError e) {
		cout << "Error occured: " << e.getMessage() << endl;;
	} catch (ErrorResultError error) {
		cout << "ErrorResult: ";
		cout << "Code: \"" << error.getErrorCode() << "\" ";
		cout << "CodeString: \"" << error.getErrorCodeString() << "\" ";
		cout << "ErrorString: \"" << error.getErrorString() << "\"" << endl;
	} catch (ResponseParseError e) {
		cout << e.getMessage() << endl;
	} catch (XmlMarshalError e) {
		cout << e.getMessage() << endl;
	}

	if (nsresult)
		delete nsresult;


	nsresult = NULL;
	return 0;
}
