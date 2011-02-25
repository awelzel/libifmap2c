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


/*
 * Run a SearchRequest with depth 0 for all kinds of identifiers against the
 * server and check if we get the same identifier as the only result back.
 *
 * It's a test for the parsing part of the identifiers works correctly, assuming
 * the MAPS is good.
 *
 * The code is miserable...
 */

#include <iostream>
#include <cstdlib>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>


using namespace std;
using namespace ifmap2c;



static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}



static bool
compareAr(AccessRequest *const ar1, AccessRequest *const ar2)
{
	return !ar1->getName().compare(ar2->getName()) &&
			!ar1->getAdministrativeDomain().compare(ar2->getAdministrativeDomain());
}



static void checkAr(SSRC *const ssrc)
{
	const string arname = "The AR Name";
	const string admind = "The Administrative Domain";

	AccessRequest *ar1 = Identifiers::createAr(arname);
	AccessRequest *ar2 = Identifiers::createAr(arname, admind);

	SearchRequest *sreq1 = Requests::createSearchReq(NULL, 0, NULL, -1, ar1);
	SearchRequest *sreq2 = Requests::createSearchReq(NULL, 0, NULL, -1, ar2);

	SearchResult *sres1 = ssrc->search(sreq1);
	SearchResult *sres2 = ssrc->search(sreq2);

	if (sres1->getResultItems().size() != 1 || sres2->getResultItems().size() != 1) {
		cerr << "Wrong number of ResultItems in SearchResult!" << endl;
		exit(1);
	}

	// get the first resultitem
	ResultItem *ri1 = *sres1->getResultItems().begin();
	ResultItem *ri2 = *sres2->getResultItems().begin();

	AccessRequest *arrecv1 = ri1->getAccessRequest();
	AccessRequest *arrecv2 = ri2->getAccessRequest();

	if (arrecv1 == NULL || arrecv2 == NULL) {
		cerr << "No AccessRequest in ResultItem!" << endl;
		exit(1);
	} else {
		cout << "AccessRequests are\t";
		if (compareAr(ar1, arrecv1) && compareAr(ar2, arrecv2)) {
			cout << "Good!" << endl;
		} else {
			cout << "Bad!" << endl;
		}
	}

	delete sres1;
	delete sres2;
	delete sreq1;
	delete sreq2;

	// No need to delete the AccessRequest objects. They are children
	// of the SearchRequests/SearchResult objects and got deleted this way.
	// The same holds for the ResultItem objects.
	// It's an error to delete them again.
}



static bool
compareMac(MacAddress *mac1, MacAddress *mac2)
{
	return !mac1->getValue().compare(mac2->getValue()) &&
			!mac1->getAdministrativeDomain().compare(mac2->getAdministrativeDomain());
}



static void
checkMac(SSRC *const ssrc)
{
	const string mac = "aa:bb:cc:dd:ee:ff";
	const string admind = "The Administrative Domain";

	MacAddress *mac1 = Identifiers::createMac(mac);
	MacAddress *mac2 = Identifiers::createMac(mac, admind);

	SearchRequest *sreq1 = Requests::createSearchReq(NULL, 0, NULL, -1, mac1);
	SearchRequest *sreq2 = Requests::createSearchReq(NULL, 0, NULL, -1, mac2);

	SearchResult *sres1 = ssrc->search(sreq1);
	SearchResult *sres2 = ssrc->search(sreq2);

	if (sres1->getResultItems().size() != 1 || sres2->getResultItems().size() != 1) {
		cerr << "Wrong number of ResultItems in SearchResult!" << endl;
		exit(1);
	}

	// get the first resultitem
	ResultItem *ri1 = *sres1->getResultItems().begin();
	ResultItem *ri2 = *sres2->getResultItems().begin();

	MacAddress *macrecv1 = ri1->getMacAddress();
	MacAddress *macrecv2 = ri2->getMacAddress();

	if (macrecv1 == NULL || macrecv2 == NULL) {
		cerr << "No MacAddress in ResultItem!" << endl;
		exit(1);
	} else {
		cout << "MacAddresses are\t";
		if (compareMac(mac1, macrecv1) && compareMac(mac2, macrecv2)) {
			cout << "Good!" << endl;
		} else {
			cout << "Bad!" << endl;
		}
	}

	delete sres1;
	delete sres2;
	delete sreq1;
	delete sreq2;
}

static bool
compareDev(Device *dev1, Device  *dev2)
{
	return !dev1->getValue().compare(dev2->getValue());
}



static void
checkDev(SSRC *const ssrc)
{
	const string aikname = "iDontKnowHowItLooksLike";
	const string devname = "NameOfTheDevice";

	Device *dev1 = Identifiers::createDev(aikname);

	SearchRequest *sreq1 = Requests::createSearchReq(NULL, 0, NULL, -1, dev1);

	SearchResult *sres1 = ssrc->search(sreq1);

	if (sres1->getResultItems().size() != 1) {
		cerr << "Wrong number of ResultItems in SearchResult!" << endl;
		exit(1);
	}

	// get the first resultitem
	ResultItem *ri1 = *sres1->getResultItems().begin();

	Device *devrecv1 = ri1->getDevice();

	if (devrecv1 == NULL) {
		cerr << "No Device in ResultItem!" << endl;
		exit(1);
	} else {
		cout << "Device are\t\t";
		if (compareDev(dev1, devrecv1)) {
			cout << "Good!" << endl;
		} else {
			cout << "Bad!" << endl;
		}
	}

	delete sres1;
	delete sreq1;
}


static bool
compareIp(IpAddress *ip1, IpAddress *ip2)
{
	return !ip1->getValue().compare(ip2->getValue())
		&& !ip1->getAdministrativeDomain().compare(ip2->getAdministrativeDomain())
		&& (ip1->getIpAddressType() == ip2->getIpAddressType());

}

static void
checkIPv4(SSRC *const ssrc)
{
	const string ipval = "192.168.0.1";
	const string admind = "The Administrative Domain";

	IpAddress *ip1 = Identifiers::createIPv4(ipval);
	IpAddress *ip2 = Identifiers::createIPv4(ipval,admind);

	SearchRequest *sreq1 = Requests::createSearchReq(NULL, 0, NULL, -1, ip1);
	SearchRequest *sreq2 = Requests::createSearchReq(NULL, 0, NULL, -1, ip2);

	SearchResult *sres1 = ssrc->search(sreq1);
	SearchResult *sres2 = ssrc->search(sreq2);

	if (sres1->getResultItems().size() != 1 || sres2->getResultItems().size() != 1) {
		cerr << "Wrong number of ResultItems in SearchResult!" << endl;
		exit(1);
	}

	// get the first resultitem
	ResultItem *ri1 = *sres1->getResultItems().begin();
	ResultItem *ri2 = *sres2->getResultItems().begin();

	IpAddress *iprecv1 = ri1->getIpAddress();
	IpAddress *iprecv2 = ri2->getIpAddress();

	if (iprecv1 == NULL || iprecv2 == NULL) {
		cerr << "No IpAddresses in ResultItem!" << endl;
		exit(1);
	} else {
		cout << "IPv4 Addresses are\t";
		if (compareIp(ip1, iprecv1) && compareIp(ip2, iprecv2)) {
			cout << "Good!" << endl;
		} else {
			cout << "Bad!" << endl;
		}
	}

	delete sres1;
	delete sres2;
	delete sreq1;
	delete sreq2;
}

static void
checkIPv6(SSRC *const ssrc)
{
	const string ipval = "3ffe:1900:4545:3:200:f8ff:fe21:67cf";
	const string admind = "The Administrative Domain";

	IpAddress *ip1 = Identifiers::createIPv6(ipval);
	IpAddress *ip2 = Identifiers::createIPv6(ipval,admind);

	SearchRequest *sreq1 = Requests::createSearchReq(NULL, 0, NULL, -1, ip1);
	SearchRequest *sreq2 = Requests::createSearchReq(NULL, 0, NULL, -1, ip2);

	SearchResult *sres1 = ssrc->search(sreq1);
	SearchResult *sres2 = ssrc->search(sreq2);

	if (sres1->getResultItems().size() != 1 || sres2->getResultItems().size() != 1) {
		cerr << "Wrong number of ResultItems in SearchResult!" << endl;
		exit(1);
	}

	// get the first resultitem
	ResultItem *ri1 = *sres1->getResultItems().begin();
	ResultItem *ri2 = *sres2->getResultItems().begin();

	IpAddress *iprecv1 = ri1->getIpAddress();
	IpAddress *iprecv2 = ri2->getIpAddress();

	if (iprecv1 == NULL || iprecv2 == NULL) {
		cerr << "No IpAddresses in ResultItem!" << endl;
		exit(1);
	} else {
		cout << "IPv6 Addresses are\t";
		if (compareIp(ip1, iprecv1) && compareIp(ip2, iprecv2)) {
			cout << "Good!" << endl;
		} else {
			cout << "Bad!" << endl;
		}
	}

	delete sres1;
	delete sres2;
	delete sreq1;
	delete sreq2;
}


static bool
compareIdentity(Identity *id1, Identity *id2)
{
	if (id1->getIdentityType() != id2->getIdentityType())
		return false;

	if (id1->getIdentityType() == other) {
		return !id1->getName().compare(id2->getName())
			&& !id1->getAdministrativeDomain().compare(id2->getAdministrativeDomain())
			&& !id1->getOtherTypeDef().compare(id2->getOtherTypeDef());
	} else {
		return !id1->getName().compare(id2->getName())
			&& !id1->getAdministrativeDomain().compare(id2->getAdministrativeDomain());
	}
}


#define NIDENTITIES 20
static void
checkIdentity(SSRC *const ssrc)
{
	std::string identname = "Aidentity";
	std::string hiphit = "1:2:3:4:c:d:e:f";
	std::string sipuri = "sip:ident@localhost:8842";
	std::string teluri = "+4-12-123-1234";
	std::string email = "ident@domain.tld";
	std::string kerberos = "pri/lurk@DOM";
	const string admind = "The Administrative Domain";
	const string othert = "DefinitionOfAType";

	// now that gets funny...

	// this should result everything with NULL?
	Identity *identities[NIDENTITIES] = {0};
	Identity *recvidentities[NIDENTITIES] = {0};
	SearchRequest *srequests[NIDENTITIES] = {0};
	SearchResult *sresults[NIDENTITIES] = {0};
	ResultItem  *resultitems[NIDENTITIES] = {0};

	identities[0] = Identifiers::createIdentity(aik_name, identname);
	identities[1] = Identifiers::createIdentity(aik_name, identname, admind);
	identities[2] = Identifiers::createIdentity(distinguished_name, identname);
	identities[3] = Identifiers::createIdentity(distinguished_name, identname, admind);
	identities[4] = Identifiers::createIdentity(dns_name, identname);
	identities[5] = Identifiers::createIdentity(dns_name, identname, admind);
	identities[6] = Identifiers::createIdentity(email_address, identname);
	identities[7] = Identifiers::createIdentity(email_address, identname, admind);
	identities[8] = Identifiers::createIdentity(hip_hit, hiphit);
	identities[9] = Identifiers::createIdentity(hip_hit, hiphit, admind);
	identities[10] = Identifiers::createIdentity(kerberos_principal, kerberos);
	identities[11] = Identifiers::createIdentity(kerberos_principal, kerberos, admind);
	identities[12] = Identifiers::createIdentity(username, identname);
	identities[13] = Identifiers::createIdentity(username, identname, admind);
	identities[14] = Identifiers::createIdentity(sip_uri, sipuri);
	identities[15] = Identifiers::createIdentity(sip_uri, sipuri, admind);
	identities[16] = Identifiers::createIdentity(tel_uri, teluri);
	identities[17] = Identifiers::createIdentity(tel_uri, teluri, admind);
	identities[18] = Identifiers::createOtherIdentity(othert, identname);
	identities[19] = Identifiers::createOtherIdentity(othert,identname, admind);


	bool foundBad = false;
	for (int i = 0; i < NIDENTITIES; i++) {
		srequests[i] = Requests::createSearchReq(NULL, 0, NULL, -1, identities[i]);
		sresults[i] = ssrc->search(srequests[i]);
		if (sresults[i]->getResultItems().size() != 1)
			cerr << "Wrong number of ResultItems in SearchResult!" << endl;
		resultitems[i] = *sresults[i]->getResultItems().begin();
		recvidentities[i] = resultitems[i]->getIdentity();
		if (recvidentities[i] == NULL) {
			cerr << "No IpAddresses in ResultItem!" << endl;
			exit(1);
		}
		if (!compareIdentity(identities[i], recvidentities[i])) {
			cout << "Identity[" << i << "] is bad" << endl;
			foundBad = true;
		} else {
			//cout << "Identity[" << i << "] is good" << endl;
		}

		// free the memory
		delete srequests[i];
		delete sresults[i];
	}

	cout << "Identities are\t\t";
	if (foundBad)
		cout << "Bad!" << endl;
	else
		cout << "Good!" << endl;
}




int
main(int argc, char *argv[])
{
	if (argc != 5)
		usage(argv[0]);

	// create ssrc object which is used for synchronous communication
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);

	try {
		cout << "Doing newSession\t";
		ssrc->newSession();
		cout << "Ok!" << endl;
		checkAr(ssrc);
		checkMac(ssrc);
		checkDev(ssrc);
		checkIPv4(ssrc);
		checkIPv6(ssrc);
		checkIdentity(ssrc);

		cout << "Doing endSession\t";
		ssrc->endSession();
		cout << "Ok!" << endl;
	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "ErrorResult:" << endl;
		cerr << " " << e.getErrorCodeString() << endl;
		cerr << " " << e.getErrorString() << endl;
	} catch (XmlUnmarshalError e) {
		cerr << "UnmarshalError:" << e.getMessage() << endl;
	} catch (...) {
		cerr << "Uncatched Exception occured" << endl;
		throw;
	}

	delete ssrc;
	return 0;
}
