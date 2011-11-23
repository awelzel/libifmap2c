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

/*
 * TODO: Somebody has to tell me something better than dispatching
 *       on the name value of the type_info, please.
 */
using namespace std;

namespace ifmap2c {

void
IdentifierHandlerDispatch::registerIdentifierHandler(
		IdentifierHandler *const handler)
{
	handlers[handler->handles()->name()] = handler;
}

XmlMarshalable *
IdentifierHandlerDispatch::toXml(Identifier *const i) const
{
	IdentifierHandler *handler = handlers[typeid(*i).name()];
	
	if (!handler) throw "NO HANDLER HERE";

	return handler->toXml(i);
}

Identifier *
IdentifierHandlerDispatch::fromXml(XmlMarshalable *const xml) const
{
	/*
	 * Go through all registered handlers and try to use
	 * fromXml(). They should return NULL if they cannot
	 * handle this instance or throw an if there is an error
	 * during parsing.
	 */
	Identifier *ret = NULL;
	map<std::string, IdentifierHandler *>::const_iterator it, end;

	it = handlers.begin();
	end = handlers.end();

	for (/* */; it != end; it++) {
		ret = (*it).second->fromXml(xml);

		if (ret)
			return ret;
	}

	throw "NO APPROPIATE IDENTIFIERHANDLER";
}
} // namespace
