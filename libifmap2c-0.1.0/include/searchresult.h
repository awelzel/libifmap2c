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

#ifndef SEARCHRESULT_H_
#define SEARCHRESULT_H_
#include "resultitem.h"
#include <string>
#include <list>

namespace ifmap2c {

class SearchResult {
public:
	SearchResult(const std::string& name = "");
	virtual ~SearchResult();

	void addResultItem(ResultItem *const resultItem);

	const std::list<ResultItem *>& getResultItems(void) const;

	const std::string& getSearchResultName(void) const;

	/**
	 * Search the list of ResultItems in the SearchResult for one
	 * that matches exactly the given Identifiers.
	 * *Should* only find one ResultItem... 
	 */
	ResultItem *
	getResultItem(Identifier *const i1, Identifier *const i2 = NULL) const;
	
	/**
	 * Search the list of ResultItems in the SearchResult for one
	 * or more which matche the types given by i1, i2.
	 * The content of the identifiers is not compared.
	 */
	std::list<ResultItem *>
	getResultItemsByType(Identifier *const i1, Identifier *const i2 = NULL) const;

private:
	std::list<ResultItem *> _resultItems;
	std::string _searchResultName;

};

} // namespace
#endif /* SEARCHRESULT_H_ */
