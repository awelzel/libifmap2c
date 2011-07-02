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
#include <list>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include "common.h"

// libifmap2c namespace
using namespace ifmap2c;

using namespace std;

typedef pair<string, string> STRP;


static void usage(const char *prog)
{
	cerr << "usage: " << prog << INDEPENDENT_USAGE_STRING << endl;
}

/*
 * Creating a lot of metadata objects, just for testing, does not make any
 * real sense
 */
int main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;
	SSRC *ssrc;
	XmlMarshalable *ardev, *arip, *armac, *ipmac, *authas,
		       *authby, *devip, *cap, *cap2, *devattr,
		       *devchar, *discby, *role1, *role2;
	Identifier *ar, *dev, *ip, *mac, *id;
	list<XmlMarshalable *> caps, roles;
	list<PublishElement *> pelist;
	PublishRequest *pr;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	
	dev = Identifiers::createDev("TheDev");
	ar = Identifiers::createAr("TheAR");
	ip = Identifiers::createIPv4("192.168.2.50");
	mac = Identifiers::createMac("aa:12:cc:23:ee:45");
	id = Identifiers::createIdentity(username, "user");

	ardev = Metadata::createArDev();
	arip = Metadata::createArIp();
	armac = Metadata::createArMac();
	ipmac = Metadata::createIpMac();
	authas = Metadata::createAuthAs();
	authby = Metadata::createAuthBy();
	devip = Metadata::createDevIp();
	cap = Metadata::createCapability("finance");
	cap2 = Metadata::createCapability("finance", "myadm");
	devattr = Metadata::createDevAttr("AntiVirusRunning");
	devchar = Metadata::createDevChar("22.10.09", "122345", "fool",
					  "Intel", NULL, "Linux", NULL,
					  "Desktop");
	discby = Metadata::createDiscoveredBy();
	role1 = Metadata::createRole("user", "domain");
	role2 = Metadata::createRole("admin");

	caps.push_back(cap);
	caps.push_back(cap2);
	roles.push_back(role1);
	roles.push_back(role2);

	pelist.push_back(Requests::createPublishUpdate(caps,
				ar->clone()));
	pelist.push_back(Requests::createPublishUpdate(ardev,
				ar->clone(), dev->clone()));
	pelist.push_back(Requests::createPublishUpdate(arip,
				ar->clone(), ip->clone()));
	pelist.push_back(Requests::createPublishUpdate(armac,
				ar->clone(), mac->clone()));
	pelist.push_back(Requests::createPublishUpdate(ipmac,
				ip->clone(), mac->clone()));
	pelist.push_back(Requests::createPublishUpdate(authas,
				ar->clone(), id->clone()));
	pelist.push_back(Requests::createPublishUpdate(authby,
				ar->clone(), dev->clone()));
	pelist.push_back(Requests::createPublishUpdate(devip,
				dev->clone(), ip->clone()));
	pelist.push_back(Requests::createPublishUpdate(devattr,
				ar->clone(), dev->clone()));
	pelist.push_back(Requests::createPublishUpdate(devchar,
				ar->clone(), dev->clone()));
	pelist.push_back(Requests::createPublishUpdate(discby,
				ip->clone(), dev->clone()));
	pelist.push_back(Requests::createPublishUpdate(roles,
				ar->clone(), id->clone()));

	pr = Requests::createPublishReq(pelist);
	pr->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	pelist.clear();

	try {
		ssrc->newSession();
		ssrc->publish(pr);
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}

	delete pr;
	delete ar; delete dev; delete ip; delete mac; delete id;
	delete ssrc;
	return 0;
}
