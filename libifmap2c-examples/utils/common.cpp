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

#include "common.h"
#include <iostream>

using namespace std;

extern "C" {
	#include <stdlib.h>
	#include <string.h>
};

static void
checkArgc(int argc, int app, void (*usage)(const char *), const char *name)
{
	if (argc != app && argc != (app + 4))
		usage(name);
}

static void
loadEnvParameters(char **url, char **user, char **pass, char **capath)
{
	*url = getenv(IFMAP_URL);
	*user = getenv(IFMAP_USER);
	*pass = getenv(IFMAP_PASSWORD);
	*capath = getenv(IFMAP_CAPATH);
	return;
}

static void
loadCmdParameters(char **s, char **url, char **user, char **pass, char **capath)
{
	*url = s[0];
	*user = s[1];
	*pass = s[2];
	*capath = s[3];
	return;
}

void checkAndLoadParameters(
		int argc,
		char **argv,
		int app,
		void (*usage)(const char *),
		char **url,
		char **user,
		char **pass,
		char **capath)
{
	*url = *user = *pass = *capath = NULL;
	checkArgc(argc, app, usage, argv[0]);

	if (argc == (app + 4))
		loadCmdParameters(&argv[app], url, user, pass, capath);
	else 
		loadEnvParameters(url, user, pass, capath);
	
	if (!*url || !*user || !*pass || !*capath) {
		cerr << "Failed to load parameters\n";
		usage(argv[0]);
	}
	return;
}

void checkUpdateOrDelete(char *str, void (*usage)(const char *),
		char *name)
{
	if (strcmp(str, "update") != 0 && strcmp(str, "delete") != 0)
		usage(name);
}

bool isUpdate(char *str)
{
	return !strcmp(str, "update");
}
