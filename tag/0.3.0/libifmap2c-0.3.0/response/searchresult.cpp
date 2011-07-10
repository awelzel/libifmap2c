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

#include "searchresult.h"
#include "typedefs.h"

using namespace std;

namespace ifmap2c {

SearchResult::SearchResult(const string& name) :
	_name(name)
{ }

SearchResult::~SearchResult()
{
	CRILISTIT it = _resultItems.begin();
	CRILISTIT end = _resultItems.end();

	for (/* see above */; it != end; it++)
		delete *it;

	_resultItems.clear();
}

void
SearchResult::addResultItem(ResultItem *const item)
{
	_resultItems.push_back(item);
}

const RILIST&
SearchResult::getResultItems(void) const
{
	return _resultItems;
}

const string&
SearchResult::getName(void) const
{
	return _name;
}

RILIST
SearchResult::getResultItemsByIdentifier(
		Identifier *const i1,
		Identifier *const i2) const
{
	CRILISTIT it = _resultItems.begin();
	CRILISTIT end = _resultItems.end();
	RILIST ret;
	ResultItem *ri;
	Identifier *ri1, *ri2;

	for (/* */; it != end; it++) {
		ri = *it;
		ri1 = ri->getIdentifier1();
		ri2 = ri->getIdentifier2();

		if ((Identifiers::same(i1, ri1) 
				&& Identifiers::same(i2, ri2))
			|| (Identifiers::same(i1, ri2) 
				&& Identifiers::same(i2, ri1))) {
			ret.push_back(ri);
		}
	}
	return ret;
}


RILIST
SearchResult::getResultItemsByType(Identifier *const i1,
		Identifier *const i2) const
{
	CRILISTIT it = _resultItems.begin();
	CRILISTIT end = _resultItems.end();
	RILIST ret;
	ResultItem *ri;
	Identifier *ri1, *ri2;

	for (/* see above */; it != end; it++) {
		ri = *it;
		ri1 = ri->getIdentifier1();
		ri2 = ri->getIdentifier2();

		if ((Identifiers::sameType(i1, ri1) 
				&& Identifiers::sameType(i2, ri2))
			|| (Identifiers::sameType(i1, ri2) 
				&& Identifiers::sameType(i2, ri1))) {
			ret.push_back(ri);
		}
	}
	return ret;
}

} // namespace
