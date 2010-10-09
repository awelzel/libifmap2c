/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2010, A. Welzel, <arne.welzel@gmail.com>
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

#ifndef POLLRESULT_H_
#define POLLRESULT_H_

#include "searchresult.h"

#include <list>

namespace LIBRARY_NAMESPACE {

class PollResult {
public:
	PollResult();
	virtual ~PollResult();

	const std::list<SearchResult *>& getSearchResults(void) const;
	const std::list<SearchResult *>& getUpdateResults(void) const;
	const std::list<SearchResult *>& getDeleteResults(void) const;
	const std::list<SearchResult *>& getNotifyResults(void) const;

	void addSearchResult(SearchResult *const sr);
	void addUpdateResult(SearchResult *const sr);
	void addDeleteResult(SearchResult *const sr);
	void addNotifyResult(SearchResult *const sr);

private:
	std::list<SearchResult *> _searchResults;
	std::list<SearchResult *> _updateResults;
	std::list<SearchResult *> _deleteResults;
	std::list<SearchResult *> _notifyResults;
};

} // namespace
#endif /* POLLRESULT_H_ */
