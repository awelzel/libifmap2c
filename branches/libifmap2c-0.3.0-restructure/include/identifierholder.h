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

#ifndef IDENTIFIERHOLDER_H_
#define IDENTIFIERHOLDER_H_

#include "identifier.h"
#include "xmlable.h"

namespace ifmap2c {

class IdentifierHolder : public XmlAble {

public:
	virtual ~IdentifierHolder() { };

	Identifier *getIdentifier1(void) const {
		return _i1;
	}
	
	Identifier *getIdentifier2(void) const {
		return _i2;
	}

protected:
	IdentifierHolder(Identifier *const i1,
			Identifier *const i2 = NULL) :
		_i1(i1), _i2(i2) { }

private:
	Identifier *_i1;
	Identifier *_i2;
};


} // namespace

#endif /* IDENTIFIERHOLDER_H_ */

