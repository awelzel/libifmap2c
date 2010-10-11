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
 * A sample for showing how libifmap2c can be used to implement a PDP
 * and a DHCP server or for demonstration purposes of MAP visualizations.
 */

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <list>

#include "ssrc.h"
#include "requests.h"
#include "identifiers.h"
#include "metadata.h"

using namespace std;
using namespace ifmap2c;

#define IDENTITY_NAME_PREFIX	"john.smith"
#define SWITCH_ONE_IP		"192.0.2.55"
#define SWITCH_ONE_DEV_NAME	"111:4"
#define SWITCH_TWO_IP		"192.0.2.56"
#define SWITCH_TWO_DEV_NAME	"111:5"
#define VLAN			"1234"

#define PDP_DEV_NAME		"111:0"
#define PDP_IP			"192.0.2.60"

#define AR_PREFIX		"111:"
#define AR_IP_PREFIX		"192.0.30."
#define AR_MAC_PREFIX		"aa:11:bb:22:cc:"

#define DHCP_IP			"192.0.2.7"
#define DHCP_STARTFIXED		"2010-10-20T12:21:39Z"
#define DHCP_STARTEND 		"2010-10-21T12:21:39Z"

#define PDP_BASIC_USER		"pdp"
#define PDP_BASIC_PW		"pdp"

#define DHCP_BASIC_USER		"dhcp"
#define DHCP_BASIC_PW		"dhcp"

// number of access requests to demonstrate
#define AR_NUMBER	64

// usec
#define TIMEFACTOR		0
#define PDP_SLEEP		(50 * TIMEFACTOR)
#define PDP_DHCP_SLEEP		(100 * TIMEFACTOR)
#define AR_SLEEP		(200 * TIMEFACTOR)

#define RENEW_SESSON_SEEP	(500 * TIMEFACTOR)

// don't want to think about it right now ;-)
#define SOME_BYTES	32

static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url capath" << endl;
	exit(1);
}


/*
 * pdp publishes information about himself and the two switches
 * he knows of
 */
static void
pdpPublishInfo(SSRC *ssrc, Device *pdpDevice,
		IpAddress *pdpIp,
		Device *switchOneDevice,
		IpAddress *switchOneIp,
		Device *switchTwoDevice,
		IpAddress *switchTwoIp)
{
	list<SubPublish *> pubOps;
	XmlMarshalable *meta = Metadata::createDevIp();
	PublishRequest *pubReq;

	// prepare the operations, use the new ->clone() operation, so
	// only a copy of the identifiers is used to create the requests.
	pubOps.push_back(Requests::createPublishUpdate(meta->clone(),
			pdpDevice->clone(), session, pdpIp->clone()));

	pubOps.push_back(Requests::createPublishUpdate(meta->clone(),
			switchOneDevice->clone(), session, switchOneIp->clone()));

	pubOps.push_back(Requests::createPublishUpdate(meta->clone(),
			switchTwoDevice->clone(), session, switchTwoIp->clone()));

	pubReq = Requests::createPublishReq(pubOps);

	// add meta namespace definition
	pubReq->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	ssrc->publish(pubReq);

	delete pubReq;
	// we used meta->clone() therefore we have to delete this instance
	// explicit
	delete meta;
}


int main(int argc, char *argv[])
{
	int arVal = 10;
	int macVal = 0;
	int ipVal = 1;
	int idVal = 1;
	int switchPort = 1;

	char arStr[SOME_BYTES];
	char macStr[SOME_BYTES];
	char ipStr[SOME_BYTES];
	char idStr[SOME_BYTES];
	char portStr[SOME_BYTES];

	if (argc != 3)
		usage(argv[0]);

	// initialize two differen SSRCs for the PDP and DHCP
	SSRC *ssrcPDP = SSRC::createSSRC(argv[1], PDP_BASIC_USER,
			PDP_BASIC_PW ,argv[2]);

	SSRC *ssrcDHCP= SSRC::createSSRC(argv[1], DHCP_BASIC_USER,
			DHCP_BASIC_PW ,argv[2]);

	Device *pdpDevice = Identifiers::createDevName(PDP_DEV_NAME);
	Device *switchOneDevice = Identifiers::createDevName(SWITCH_ONE_DEV_NAME);
	Device *switchTwoDevice = Identifiers::createDevName(SWITCH_TWO_DEV_NAME);
	IpAddress *pdpIp = Identifiers::createIPv4(PDP_IP);
	IpAddress *switchOneIp = Identifiers::createIPv4(SWITCH_ONE_IP);
	IpAddress *switchTwoIp = Identifiers::createIPv4(SWITCH_TWO_IP);

	try {
		ssrcPDP->newSession();
		ssrcDHCP->newSession();

		pdpPublishInfo(ssrcPDP, pdpDevice, pdpIp, switchOneDevice, switchOneIp,
				switchTwoDevice, switchTwoIp);

		// those aren't needed anymore
		delete switchOneIp;
		delete switchTwoIp;
		delete pdpIp;
		switchOneIp = NULL;
		switchTwoIp = NULL;
		pdpIp = NULL;
		Device *curSwitch = switchOneDevice;

		while (ipVal <= AR_NUMBER) {
			// prepare some string
			snprintf(arStr, SOME_BYTES, "%s%d", AR_PREFIX, arVal);
			snprintf(macStr, SOME_BYTES, "%s%02hhx", AR_MAC_PREFIX, macVal);
			snprintf(ipStr, SOME_BYTES, "%s%d", AR_IP_PREFIX, ipVal);
			snprintf(idStr, SOME_BYTES, "%s%d", IDENTITY_NAME_PREFIX, idVal);
			snprintf(portStr, SOME_BYTES, "%d", switchPort);

			cout << "DEBUG: " << arStr << "  " << macStr << "  ";
			cout << ipStr << "   " << idStr << "   " << portStr << endl;

			// our local identifiers
			AccessRequest *ar = Identifiers::createAr(arStr);
			MacAddress *mac = Identifiers::createMac(macStr);
			IpAddress *ip = Identifiers::createIPv4(ipStr);
			Identity *id = Identity::createIdentity(username, idStr);

			// bit crazy?
#define PDP_REQUESTS	5

			PublishRequest *pdpPublishRequests[PDP_REQUESTS];
			pdpPublishRequests[0] = Requests::createPublishReq(
					Requests::createPublishUpdate(
					Metadata::createRole(), ar->clone(),
					session, id->clone()));

			pdpPublishRequests[1] = Requests::createPublishReq(
					Requests::createPublishUpdate(
					Metadata::createAuthAs(), ar->clone(),
					session, id->clone()));

			pdpPublishRequests[2] = Requests::createPublishReq(
					Requests::createPublishUpdate(
					Metadata::createArMac(), ar->clone(),
					session, mac->clone()));

			pdpPublishRequests[3] = Requests::createPublishReq(
					Requests::createPublishUpdate(
					Metadata::createAuthBy(), ar->clone(),
					session, pdpDevice->clone()));

			pdpPublishRequests[4] = Requests::createPublishReq(
					Requests::createPublishUpdate(
					Metadata::createLayer2Info(VLAN, NULL, portStr, NULL),
					ar->clone(), session, curSwitch->clone()));

			for (int i = 0; i < PDP_REQUESTS; i++)
				pdpPublishRequests[i]->addXmlNamespaceDefinition(TCG_META_NSPAIR);

			PublishRequest *ipmac = Requests::createPublishReq(
					Requests::createPublishUpdate(
					Metadata::createIpMac(), mac->clone(),
					session, ip->clone()));
			ipmac->addXmlNamespaceDefinition(TCG_META_NSPAIR);


			// now we can start publish the stuff...

			for (int i = 0; i < PDP_REQUESTS; i++) {
				ssrcPDP->publish(pdpPublishRequests[i]);
				usleep(PDP_SLEEP);
			}
			usleep(PDP_DHCP_SLEEP);
			ssrcDHCP->publish(ipmac);
			usleep(AR_SLEEP);

			// cleanup :-(
			delete ar;
			delete mac;
			delete ip;
			delete id;

			for (int i = 0; i < 5; i++)
				delete pdpPublishRequests[i];

			delete ipmac;


			// end of run...
			arVal++; ipVal++; macVal++; idVal++;
			curSwitch = (curSwitch == switchOneDevice) ? switchTwoDevice : switchOneDevice;
			switchPort = (curSwitch == switchOneDevice) ? switchPort : switchPort + 1;
		}

		// idle around, could bulid in some signal handling for strg+c ?
		while (false) {
			ssrcDHCP->renewSession();
			ssrcPDP->renewSession();
			usleep(RENEW_SESSON_SEEP);
		}

		ssrcDHCP->endSession();
		ssrcPDP->endSession();

	} catch (CommunicationError e) {
		cerr << "CommunicationError: " << e.getMessage() << endl;
	} catch (ErrorResultError e) {
		cerr << "ErrorResult:" << endl;
		cerr << " " << e.getErrorCodeString() << endl;
		cerr << " " << e.getErrorString() << endl;
	} catch (...) {
		cerr << "Unidentfied Exception" << endl;
		throw;
	}

	delete pdpDevice;
	delete switchOneDevice;
	delete switchTwoDevice;

	delete ssrcDHCP;
	delete ssrcPDP;

	return 0;
}
