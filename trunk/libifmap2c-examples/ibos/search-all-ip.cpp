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
 * Check whether we can use the all-ip global identifier of IBOS
 */

#include <iostream>
#include <string>

// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/basicxmlmarshalable.h>

#include "common.h"

// make life easier
using namespace ifmap2c;
using namespace std;

#define GID_PREFIX "gid"
#define GID_HREF   "https://192.168.1.20/ifmap-schema/global-identifiers"
//#define GID_HREF   "http://myidents"

#define STRP pair<string, string>

class AllIp : public Identifier {

public:
	// Uhh... :(
	Identifier *clone(void) const { return new AllIp(); }
};

class AllIpHandler : public IdentifierHandler {


	XmlMarshalable *toXml(Identifier *const i) {
		Identifier *ix =i;
		ix = NULL;
		XmlMarshalable *ret = new BasicXmlMarshalable("all-ip", EMPTY_VALUE,
				STRP(GID_PREFIX, GID_HREF));
		ret->addXmlNamespaceDefinition(STRP(GID_PREFIX, GID_HREF));

		return ret;
	}

	AllIp *fromXml(XmlMarshalable *const i) {
		(void)i;
		throw RequestHandlerError("UNUSED?!");
		return NULL;
	}

	bool canHandle(Identifier *const i) const {
		return typeid(*i) == typeid(AllIp);
	}
};

static void usage(const char *prog)
{
	cerr << "usage: " << prog << " "
		INDEPENDENT_USAGE_STRING << endl;
}

int main(int argc, char* argv[])
{
	char *url, *user, *pass, *capath;
	SSRC *ssrc = NULL;
	SearchRequest *sreq = NULL;
	SearchResult *sres = NULL;
	Identifier *allip = new AllIp();
	IdentifierHandlerDispatch::registerHandler(new AllIpHandler());

	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);
	sreq = Requests::createSearchReq(
			FILTER_MATCH_ALL,
			2,
			FILTER_MATCH_ALL,
			SEARCH_NO_MAX_RESULT_SIZE,
			SEARCH_NO_TERMINAL_IDENTIFIERS,
			allip);

	try {	
		ssrc->newSession();

		sres = ssrc->search(sreq);

		ssrc->endSession();

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	delete sres;
	delete sreq;
	delete ssrc;

	return 0;
}
