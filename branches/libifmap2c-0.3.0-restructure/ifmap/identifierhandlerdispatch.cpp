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

#include "identifier.h"
#include "ifmap/baseidentifierhandler.h"

/*
 * TODO: Somebody has to give me a good idea how to do better
 *       dispatching :-/
 */
using namespace std;

namespace ifmap2c {

static list<IdentifierHandler *> baseHandlers(void)
{
	list<IdentifierHandler *> ret;
	ret.push_back(IFMAP2C_IH_CREATE_CALL(AccessRequest));
	ret.push_back(IFMAP2C_IH_CREATE_CALL(Device));
	ret.push_back(IFMAP2C_IH_CREATE_CALL(Identity));
	ret.push_back(IFMAP2C_IH_CREATE_CALL(IpAddress));
	ret.push_back(IFMAP2C_IH_CREATE_CALL(MacAddress));

	return ret;
}

list<IdentifierHandler *> IfmapIdentifierHandlerDispatch::handlers(baseHandlers());



void
IfmapIdentifierHandlerDispatch::registerIdentifierHandler(
		IdentifierHandler *const handler)
{
	handlers.push_back(handler);
}

XmlMarshalable *
IfmapIdentifierHandlerDispatch::toXml(Identifier *const i) const
{
	list<IdentifierHandler *>::const_iterator it, end;
	it = handlers.begin();
	end = handlers.end();

	for (/* */; it != end; it++)
		if ((*it)->canHandle(i))
			return (*it)->toXml(i);
	
	throw "NO IDENTIFIER HANDLER FOUND";
}

Identifier *
IfmapIdentifierHandlerDispatch::fromXml(XmlMarshalable *const xml) const
{
	/*
	 * Go through all registered handlers and try to use
	 * fromXml(). They should return NULL if they cannot
	 * handle this instance or throw an if there is an error
	 * during parsing.
	 */
	Identifier *ret = NULL;
	list<IdentifierHandler *>::const_iterator it, end;

	it = handlers.begin();
	end = handlers.end();

	for (/* */; it != end; it++) {
		ret = (*it)->fromXml(xml);

		if (ret)
			return ret;
	}

	throw "NO APPROPIATE IDENTIFIERHANDLER";
}
} // namespace
