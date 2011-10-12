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

// libifmap2c includes
#include <libifmap2c/ssrc.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <cstdio>

#include "common.h"

using namespace std;
using namespace ifmap2c;

typedef pair<string, string> STRP;
typedef list<SearchResult *> SRLIST;
typedef list<ResultItem *> RILIST;
typedef list<XmlMarshalable *> XMLMLIST;


static void
usage(const char *name)
{
	cerr << "Usage: " << name << " <number> "
		INDEPENDENT_USAGE_STRING << endl;
}

int
main(int argc, char *argv[])
{
	int i, total;
	SSRC *ssrc = NULL;
	char *url, *user, *pass, *capath;

	checkAndLoadParameters(argc, argv, 1, usage, &url, &user, &pass,
			&capath);
	
	total = atoi(argv[1]);
	
	ssrc = SSRC::createSSRC(url, user, pass, capath);


	try {
		ssrc->newSession();

		for (i = 0; i < total; i++) {
			ssrc->renewSession();
			cout << ".";
			cout.flush();
		}
		cout << endl;

		ssrc->endSession();

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}
	delete ssrc;
}
