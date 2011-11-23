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

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_
#include "xmlmarshalable.h"
#include <typeinfo>
#include <map>

namespace ifmap2c {

class Identifier {

protected:
	Identifier();

public:
	virtual ~Identifier();
	virtual Identifier *clone(void) const = 0;
};


class IdentifierAdmin : public Identifier {
public:
	virtual ~IdentifierAdmin();
	const std::string& getAdministrativeDomain(void) const;

protected:
	IdentifierAdmin(const std::string& ad);

private:
	std::string _administrativeDomain;
};


/**
 * Interface for IdentifierHandler
 */
class IdentifierHandler {

public:
	virtual XmlMarshalable *toXml(Identifier *const ident) = 0;
	virtual Identifier *fromXml(XmlMarshalable *const xml) = 0;
	virtual bool canHandle(Identifier * const parname) const = 0;
	virtual const std::type_info *handles(void) const = 0;
};

// Some macros to safe some typing for the IdentifierHandler thing
#define IFMAP2C_IH_NAME(type)	type##Handler

#define IFMAP2C_IH_TOXML_DEF(type, parname)			\
ifmap2c::XmlMarshalable *IFMAP2C_IH_NAME(type)::toXml(		\
		ifmap2c::Identifier *const parname)

#define IFMAP2C_IH_FROMXML_DEF(type, parname)			\
type *IFMAP2C_IH_NAME(type)::fromXml(				\
		ifmap2c::XmlMarshalable *const parname)

#define IFMAP2C_IH_TOXML_DECL(type, parname)			\
ifmap2c::XmlMarshalable *toXml(					\
		ifmap2c::Identifier *const parname)

#define IFMAP2C_IH_FROMXML_DECL(type, parname)			\
type *fromXml(ifmap2c::XmlMarshalable *const parname)

#define IFMAP2C_IH_CANHANDLE_DEF(type, parname)			\
bool canHandle(Identifier * const parname) const {		\
	return typeid(*(parname)) == typeid(type);		\
}

#define IFMAP2C_IH_HANDLES_DEF(type)				\
const std::type_info *handles(void) const {			\
	return &typeid(type);					\
}

#define IFMAP2C_IH_HEADER(type)					\
class IFMAP2C_IH_NAME(type) : public ifmap2c::IdentifierHandler {\
public:								\
	IFMAP2C_IH_TOXML_DECL(type, param);			\
	IFMAP2C_IH_FROMXML_DECL(type, param);			\
	IFMAP2C_IH_CANHANDLE_DEF(type, param);			\
	IFMAP2C_IH_HANDLES_DEF(type);				\
};


class IdentifierHandlerDispatch {

public:
	/**
	 * Register a new IdentifierHandler
	 */
	static void
	registerIdentifierHandler(IdentifierHandler *const handler);

	/**
	 * Dispatch XML generation to an appropiate IdentifierHandler.
	 *
	 * Find the responsible IdentifierHandler and delegate
	 * the call accordingly to generate something we can make
	 * XML out of
	 */
	XmlMarshalable *toXml(Identifier *const i) const;

	/**
	 * Try each registered IdentifierHandler to create an
	 * Identifier from the given XML
	 */
	Identifier *fromXml(XmlMarshalable *const xml) const;

private:
	static std::map<std::string, IdentifierHandler *> handlers;
};

} // namespace
#endif /* IDENTIFIER_H_ */
