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

#include "pollresult.h"
#include "typedefs.h"

using namespace std;

namespace ifmap2c {

PollResult::PollResult()
{ }

PollResult::~PollResult()
{
	SearchResult *child = NULL;
	ErrorResultError *error = NULL;

	SRLISTIT it = _searchResults.begin();
	SRLISTIT end = _searchResults.end();
	ERLISTIT iterr = _errorResults.begin();
	ERLISTIT enderr = _errorResults.end();

	for (child = *it; it != end; child = *(++it))
		delete child;
	_searchResults.clear();

	it = _updateResults.begin();
	end = _updateResults.end();
	for (child = *it; it != end; child = *(++it))
		delete child;
	_updateResults.clear();

	it = _deleteResults.begin();
	end = _deleteResults.end();
	for (child = *it; it != end; child = *(++it))
		delete child;
	_deleteResults.clear();

	it = _notifyResults.begin();
	end = _notifyResults.end();
	for (child = *it; it != end; child = *(++it))
		delete child;
	_notifyResults.clear();
	
	for (error = *iterr; iterr != enderr; error = *(++iterr))
		delete error;
	_errorResults.clear();
}

const list<SearchResult *>&
PollResult::getSearchResults(void) const
{
	return _searchResults;
}

const list<SearchResult *>&
PollResult::getUpdateResults(void) const
{
	return _updateResults;
}

const list<SearchResult *>&
PollResult::getDeleteResults(void) const
{
	return _deleteResults;
}

const list<SearchResult *>&
PollResult::getNotifyResults(void) const
{
	return _notifyResults;
}

const list<ErrorResultError *>&
PollResult::getErrorResults(void) const
{
	return _errorResults;
}

void
PollResult::addSearchResult(SearchResult *const sr)
{
	_searchResults.push_back(sr);
}

void
PollResult::addUpdateResult(SearchResult *const sr)
{
	_updateResults.push_back(sr);
}

void
PollResult::addDeleteResult(SearchResult *const sr)
{
	_deleteResults.push_back(sr);
}

void
PollResult::addNotifyResult(SearchResult *const sr)
{
	_notifyResults.push_back(sr);
}
void
PollResult::addErrorResult(ErrorResultError *const er)
{
	_errorResults.push_back(er);
}

} // namespace
