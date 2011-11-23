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

#ifndef REQUEST_H_
#define REQUEST_H_

#include "xmlable.h"
#include "result.h"
#include "xmlmarshalable.h"
#include <map>
#include <string>
#include <typeinfo>

namespace ifmap2c {

/**
 * Just a simple marker interface for requests...
 */
class Request : public XmlAble {

public:
	virtual ~Request() { };
	Request() { };
};

class RequestHandler {

public:
	virtual XmlMarshalable *toXml(Request *const req) = 0;
	virtual Result *fromXml(XmlMarshalable *const xml) = 0;
	virtual const std::type_info *handles(void) const = 0;
};

// Some macros to safe some typing for the RequestHandler thing
// FIXME: how to unify with IdentifierHandler? Templates?
#define IFMAP2C_RH_NAME(type)	type##Handler

#define IFMAP2C_RH_TOXML_DEF(type, parname)			\
ifmap2c::XmlMarshalable *IFMAP2C_RH_NAME(type)::toXml(		\
		ifmap2c::Request *const parname)

#define IFMAP2C_RH_FROMXML_DEF(type, parname)			\
Result *IFMAP2C_RH_NAME(type)::fromXml(				\
		ifmap2c::XmlMarshalable *const parname)

#define IFMAP2C_RH_TOXML_DECL(type, parname)			\
ifmap2c::XmlMarshalable *toXml(					\
		ifmap2c::Request *const parname)

#define IFMAP2C_RH_FROMXML_DECL(type, parname)			\
Result *fromXml(ifmap2c::XmlMarshalable *const parname)

#define IFMAP2C_RH_CANHANDLE_DEF(type, parname)			\
bool canHandle(Request * const parname) const {		\
	return typeid(*(parname)) == typeid(type);		\
}

#define IFMAP2C_RH_HANDLES_DEF(type, parname)			\
const std::type_info *handles(void) const {			\
	return &typeid(type);					\
}

#define IFMAP2C_RH_HEADER(type)					\
class IFMAP2C_RH_NAME(type) : public ifmap2c::RequestHandler {	\
public:								\
	IFMAP2C_RH_TOXML_DECL(type, param);			\
	IFMAP2C_RH_FROMXML_DECL(type, param);			\
	IFMAP2C_RH_CANHANDLE_DEF(type, param);			\
	IFMAP2C_RH_HANDLES_DEF(type, parname);			\
};


class RequestHandlerDispatch {

public:
	/**
	 * Register a new RequestHandler
	 */
	static void
	registerRequestHandler(RequestHandler *const handler);

	/**
	 * Get the appropiate RequestHandler for the given Request
	 */
	RequestHandler *dispatch(Request *const req) const;

private:
	static std::map<std::string, RequestHandler*> handlers;
};

} // namespace

#endif /* REQUEST_H_*/
