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

#include "searchrequest.h"
#include "typedefs.h"
#include "tcgifmapbase.h"

#include <sstream>

using namespace std;

namespace ifmap2c {

SearchRequest *
SearchRequest::createSearchRequest(const char *const matchLinksFilter,
		const int maxDepth,
		const char *const resultFilter,
		const int maxResultSize,
		Identifier *const i1)
{
	return new SearchRequest(matchLinksFilter, maxDepth, resultFilter,
			maxResultSize, i1);
}

SearchRequest::SearchRequest(const char *const matchLinksFilter,
		const int maxDepth,
		const char *const resultFilter,
		const int maxResultSize,
		Identifier *const i1) :
		BasicXmlMarshalable( SEARCH_ELEMENT_NAME, EMPTY_VALUE,
				IFMAP_OPERATION_NSPAIR)
{
	stringstream ss;
	if (matchLinksFilter)
		addXmlAttribute(STRP(SEARCH_MATCH_LINKS_ATTR_NAME, matchLinksFilter));

	if (maxDepth >= 0) {
		ss << maxDepth;
		addXmlAttribute(STRP(SEARCH_MAX_DEPTH_ATTR_NAME, ss.str()));
		ss.clear();
	}

	if (resultFilter)
		addXmlAttribute(STRP(SEARCH_RESULT_FILTER_ATTR_NAME, resultFilter));

	if (maxResultSize >= 0) {
		ss << maxResultSize;
		addXmlAttribute(STRP(SEARCH_MAX_DEPTH_ATTR_NAME, ss.str()));
		ss.clear();
	}

	if (i1)
		addXmlChild(i1);
}

SearchRequest::~SearchRequest() { }

} // namespace
