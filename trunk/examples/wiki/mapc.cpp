/* filename mapc.cpp */
#include <iostream>
#include <list>

#include <libifmap2c/ssrc.h>
#include <libifmap2c/requests.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>


// libifmap2c namespace
using namespace ifmap2c;

using namespace std;

typedef pair<string, string> STRP;


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

	// Note:
	// To use certificate based authentication one has to use
	// SSRC::createSSRC(url, /* server url */
	// 		    key, /* private key in PEM format */
	// 		    password, /* private key password */
	// 		    certificate, /* path to certificate */
	// 		    capath /* as above */)

	// Create identifiers used for the request using the
	// Identifiers class
	IpAddress *ip1 = Identifiers::createIPv4("192.168.0.1");
	MacAddress *mac1 = Identifiers::createMac("aa:bb:cc:dd:ee:ff");
	IpAddress *ip2 = Identifiers::createIPv4("192.168.0.2");
	MacAddress *mac2 = Identifiers::createMac("aa:bb:cc:dd:ee:01");

	// create a metadata object, using the Metadata class
	XmlMarshalable *ipmac = Metadata::createIpMac();

	// need to define the standard meta namespace somewhere,
	// could also be done on both publishUpdate elements or
	// on the publish element
	ipmac->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	// create two publishUpdate elements with session lifetime
	// and two identfiers (link), we need to clone ipmac the first
	// time, else there will be the same pointer in two distinct
	// PublishUpdate objects, this would result in a double free
	// of the pointer, which is bad...
	PublishUpdate *pubUpdate1 = Requests::createPublishUpdate(ipmac->clone(),
			ip1, session, mac1);
	
	PublishUpdate *pubUpdate2 = Requests::createPublishUpdate(ipmac,
			ip2, session, mac2);

	// store the publishUpdate elements in a list
	list<SubPublish *> publishList;
	publishList.push_back(pubUpdate1);
	publishList.push_back(pubUpdate2);

	// initialize the a publish request with the elements stored
	// in the list
	PublishRequest *publishReq = Requests::createPublishReq(publishList);

	// These objects are now children of the publishReq object.
	// We shouldn't touch them... Make sure we won't.
	ip1 = NULL, ip2 = NULL, mac1 = NULL, mac2 = NULL;
	ipmac = NULL; pubUpdate1 = NULL, pubUpdate2 = NULL;

	try {
		// run a newSession request
		ssrc->newSession();

		// run the publish request
		ssrc->publish(publishReq);

		// run end session request
		ssrc->endSession();

	// CommunicationError occurs if something goes wrong with the
	// certificate or the other side is not reachable
	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;

	// ErrorResultError is thrown, if a errorResult is received from
	// the server. For example InvalidSessionID or InvalidMetadata.
	} catch (ErrorResultError e) {
		cerr << "ErrorResult  " << endl;
		cerr << " ErrorCode   " << e.getErrorCodeString() << endl;
		cerr << " ErrorString " << e.getErrorString() << endl;

	// other things can come up here, but we ignore that for now
	} catch (...) {
		cerr << "Unidentified Error." << endl;
		throw; // Throw it up to print the name
	}

	// delting the publish request
	// this will delte every child element which was added,
	// i.e both PublishUpdate objects will be deleted, which
	// in turn results in deletion of the Identifier and
	// Metadata objects. Therefore, no need to free these
	// explicitly.
	delete publishReq;

	// deleting the SSRC, this will close the TCP connection
	delete ssrc;

	return 0;
}
