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

#include "subscribeupdate.h"
#include "typedefs.h"
#include "tcgifmapbase.h"
#include <sstream>

using namespace std;

namespace ifmap2c {

SubscribeUpdate*
SubscribeUpdate::createSubscribeUpdate(const string& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1)
{
	return new SubscribeUpdate(name, matchLinksFilter, maxDepth,
			resultFilter, maxResultSize, terminalIdent, i1);
}

SubscribeUpdate::SubscribeUpdate(const string& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1) :
			SubscribeElement(SUBSCRIBE_UPDATE_ELEMENT_NAME, name)
{
	// this is the same as in a search request
	if (matchLinksFilter)
		addXmlAttribute(STRP(SEARCH_MATCH_LINKS_ATTR_NAME, matchLinksFilter));

	if (maxDepth >= 0) {
		stringstream ss;
		ss << maxDepth;
		addXmlAttribute(STRP(SEARCH_MAX_DEPTH_ATTR_NAME, ss.str()));
	}

	if (resultFilter)
		addXmlAttribute(STRP(SEARCH_RESULT_FILTER_ATTR_NAME, resultFilter));

	if (maxResultSize >= 0) {
		stringstream ss;
		ss << maxResultSize;
		addXmlAttribute(STRP(SEARCH_MAX_SIZE_ATTR_NAME, ss.str()));
	}

	if (terminalIdent) {
		addXmlAttribute(STRP(SEARCH_TERM_IDENT_ATTR_NAME, terminalIdent));
	}

	if (i1)
		addXmlChild(i1);
}

} // namespace
