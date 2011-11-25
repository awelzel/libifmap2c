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

#ifndef SEARCHPARAM_H_
#define SEARCHPARAM_H_

#include "identifier.h"

namespace ifmap2c {

class SearchParam {

public:
	const char *getMatchLinksFilter(void) const;

	int getMaxDepth(void) const;

	const char *getResultFilter(void) const;

	int getMaxResultSize(void) const;

	const char *getTerminalIdentifiers(void) const;

	Identifier *getStartIdentifier(void) const;

	SearchParam(const char *const matchLinksFilter,
		const int maxDepth,
		const char *const resultFilter,
		const int maxResultSize,
		const char *const terminalIdent,
		Identifier *const start);

	virtual ~SearchParam() { }

private:
	const char *_matchLinksFilter;

	const int _maxDepth;

	const char *_resultFilter;

	const int _maxResultSize;

	const char *_terminalIdentifiers;

	Identifier *const _startIdentifier;
};

} // namespace

#endif /* SEARCHPARAM_H_ */
