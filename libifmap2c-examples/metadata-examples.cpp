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

// libifmap2c namespace
using namespace ifmap2c;

using namespace std;

typedef pair<string, string> STRP;

/*
 * Creating a lot of metadata objects, just for testing, does not make any
 * real sense
 */
int main(int argc, char *argv[])
{
	if (argc != 5) {
		cout << "Usage: " << argv[0] << " ifmap-server-url"
				" user pass capath" << endl;
		return 1;
	}

	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3], argv[4]);

	Device *dev = Identifiers::createDev("TheDev");
	AccessRequest *ar = Identifiers::createAr("TheAR");

	XmlMarshalable *ardev = Metadata::createArDev();
	XmlMarshalable *arip = Metadata::createArIp();
	XmlMarshalable *armac = Metadata::createArMac();
	XmlMarshalable *ipmac = Metadata::createIpMac();
	XmlMarshalable *authas = Metadata::createAuthAs();
	XmlMarshalable *authby = Metadata::createAuthBy();
	XmlMarshalable *devip = Metadata::createDevIp();
	XmlMarshalable *cap = Metadata::createCapability("finance");
	XmlMarshalable *cap2 = Metadata::createCapability("finance", "myadm");
	XmlMarshalable *devattr = Metadata::createDevAttr("AntiVirusRunning");
	XmlMarshalable *devchar = Metadata::createDevChar(
			"22.10.09", "122345", "fool", "Intel", NULL, "Linux",
			NULL, "Desktop");
	XmlMarshalable *discby = Metadata::createDiscoveredBy();
	XmlMarshalable *role1 = Metadata::createRole("user", "domain");
	XmlMarshalable *role2 = Metadata::createRole("admin");

	list<XmlMarshalable *> metalist;
	metalist.push_back(ardev);
	metalist.push_back(arip);
	metalist.push_back(armac);
	metalist.push_back(ipmac);
	metalist.push_back(authas);
	metalist.push_back(authby);
	metalist.push_back(devip);
	metalist.push_back(cap);
	metalist.push_back(cap2);
	metalist.push_back(devattr);
	metalist.push_back(devchar);;
	metalist.push_back(discby);;
	metalist.push_back(role1);
	metalist.push_back(role2);

	PublishUpdate *pubUpdate = Requests::createPublishUpdate(metalist, ar, session, dev);
	PublishRequest *publishReq = Requests::createPublishReq(pubUpdate);
	publishReq->addXmlNamespaceDefinition(TCG_META_NSPAIR);

	try {
		ssrc->newSession();
		ssrc->publish(publishReq);
		ssrc->endSession();
	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}

	delete publishReq;
	delete ssrc;
	
	return 0;
}
