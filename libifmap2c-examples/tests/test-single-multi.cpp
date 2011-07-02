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

/*
 * Check whether we get exceptions if we specify contradictionary
 * metdata. Further, demonstrates a bit how to create user-specific
 * metadata
 *
 * This might give MAPS specific results, depending on whether the
 * MAPS really supports namespaces...
 */


// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <list>

#include "common.h"


using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;

static void
usage(const char *name)
{
	cerr << "Usage: " << name << INDEPENDENT_USAGE_STRING << endl;
}

int
main(int argc, char *argv[])
{
	SSRC *ssrc;
	char *url, *user, *pass, *capath;
	XmlMarshalable *sin1, *mul1, *sin2, *mul2;
	PublishRequest *pr1, *pr2, *pr3, *pr4;
	Identifier *ip;

	checkAndLoadParameters(argc, argv, 0, usage, &url, &user, &pass,
			&capath);

	ssrc = SSRC::createSSRC(url, user, pass, capath);

	sin1 = new UserMetadata("simpl", "prx1", "http://href1", single);
	sin1->addXmlNamespaceDefinition(STRP("prx1", "http://href1"));

	mul1 = new UserMetadata("simpl", "prx3", "http://href1", multi);
	mul1->addXmlNamespaceDefinition(STRP("prx3", "http://href1"));

	sin2 = new UserMetadata("simpl", "ns1", "http://href2", single);
	sin2->addXmlNamespaceDefinition(STRP("ns1", "http://href2"));

	mul2 = new UserMetadata("simpl", "ns2", "http://href2", multi);
	mul2->addXmlNamespaceDefinition(STRP("ns2", "http://href2"));

	ip = IpAddress::createIpv4Address("192.168.0.1");

	pr1 = Requests::createPublishReq(
			Requests::createPublishUpdate(sin1, ip->clone()));
	pr2 = Requests::createPublishReq(
			Requests::createPublishUpdate(mul1, ip->clone()));
	pr3 = Requests::createPublishReq(
			Requests::createPublishUpdate(mul2, ip->clone()));
	pr4 = Requests::createPublishReq(
			Requests::createPublishUpdate(sin2, ip));


	try {
		ssrc->newSession();
		ssrc->publish(pr1);
		try {
			ssrc->publish(pr2);
			cerr << "Publishing multiValue allowed" << endl;
		} catch (ErrorResultError e) {
			// that's good
		}

		ssrc->publish(pr3);
		try {
			ssrc->publish(pr4);
			cerr << "Publishing singleValue allowed" << endl;
		} catch (ErrorResultError e) {
			// that's good
		}
		ssrc->endSession();

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResultError e) {
		cerr << e << endl;
	}

	delete pr1; delete pr2; delete pr3; delete pr4;
	delete ssrc;
	return 0;
}
