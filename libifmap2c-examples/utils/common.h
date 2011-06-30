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
#ifndef COMMON_H_
#define COMMON_H_

#define IFMAP_URL 			"IFMAP_URL"
#define IFMAP_USER 			"IFMAP_USER"
#define IFMAP_PASSWORD			"IFMAP_PASSWORD"
#define IFMAP_CAPATH			"IFMAP_CAPATH"

#define INDEPENDENT_USAGE_STRING						\
		" [url user password capath]\n\n"				\
		"You may also use the enviroment variables " IFMAP_URL		\
		", " IFMAP_USER ",\n" IFMAP_PASSWORD " and " IFMAP_CAPATH	\
		" instead of the corresponding parameters."			\


void checkAndLoadParameters(
		int argc,
		char **argv,
		int app,
		void (*usage)(const char *),
		char **url,
		char **user,
		char **pass,
		char **capath);

void checkUpdateOrDelete(
		char *str,
		void (*usage)(const char *),
		char *name);

bool isUpdate(char *str);
		

#endif // COMMON_H_
