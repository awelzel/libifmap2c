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

/* Performance measurement of random metadata types */


/*
 * Publish random generated metadata types #request times to
 * #ips randomly chosen ipaddresses. Use #perrequest different
 * ip / metadata elements per publish request. Chose between
 * #metadata_types different metadata types.
 */
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>
#include <time.h>

#include "common.h"
#include "testcommon.h"

using namespace std;
using namespace ifmap2c;

static void
usage(const char *name)
{
	cerr << "Usage: " << name << " <#requests> <#perrequest> <#ips> <#metadata_types>"
		INDEPENDENT_USAGE_STRING << endl;
}


typedef pair<string, string> STRP;
static STRP mdNs("myns", "http://mynamespace.com");

/* A metadata class with variable element name and cardinality multiValue */
class MyMetadata : public BasicXmlMarshalable {
public:
	MyMetadata(const string &elName) :
		BasicXmlMarshalable(elName, "", mdNs)
	{
		addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, "multiValue"));
		addXmlNamespaceDefinition(mdNs);
	}
};


int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath, tmpBuf[64];
	checkAndLoadParameters(argc, argv, 4, usage, &url, &user, &pass,
			&capath);

	int countRequests = atoi(argv[1]);
	int perRequest = atoi(argv[2]);
	int countIps = atoi(argv[3]);
	int countMdTypes = atoi(argv[4]);

	if (countIps > 65536 || countIps < 0) {
		cerr << "IP count is limited to 65536." << endl;
		exit(1);
	}
	
	if (countMdTypes > 65536 || countMdTypes < 0) {
		cerr << "Metadata type count is limited to 65536." << endl;
		exit(1);
	}

	SSRC *ssrc = SSRC::createSSRC(url, user, pass, capath);
	PublishRequest *pr;
	PublishElement *up;

	IpAddress *ips[countIps];
	XmlMarshalable *mds[countMdTypes];

	/* prepare all IP addresses */
	for (int i = 0; i < countIps; i++) {
		
		snprintf(tmpBuf, sizeof(tmpBuf), "192.168.%d.%d",
				(i / 256) % 256, i % 256);
		ips[i] = Identifiers::createIPv4(tmpBuf);
	}
	
	/* prepare all metadata types addresses */
	for (int i = 0; i < countMdTypes; i++) {
		
		snprintf(tmpBuf, sizeof(tmpBuf), "mdType%d", i);
		mds[i] = new MyMetadata(tmpBuf);
	}

	srandom(time(NULL));

	try {
		ssrc->newSession();
		for (int i = 0; i < countRequests; i++) {


			list<PublishElement *> plist;
			for (int j = 0; j < perRequest; j++) {
				int randIp = random() % countIps;
				int randMd = random() % countMdTypes;


				up = Requests::createPublishUpdate(
						mds[randMd]->clone(),
						ips[randIp]->clone());
				plist.push_back(up);
			}

			pr = Requests::createPublishReq(plist);
			plist.clear();

			ssrc->publish(pr);

			delete pr;
			pr = NULL;
		}

		ssrc->endSession();

	} catch (XmlCommunicationError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}
	
	for (int i = 0; i < countIps; i++)
		delete ips[i];
	
	for (int i = 0; i < countMdTypes; i++)
		delete mds[i];

	delete ssrc;
}
