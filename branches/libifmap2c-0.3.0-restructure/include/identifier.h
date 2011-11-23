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


class IdentifierHandler {

public:
	virtual XmlMarshalable *toXml(Identifier *const ident) = 0;
	virtual Identifier *fromXml(XmlMarshalable *const xml) = 0;
};

// Some macros to safe some typing for the IdentifierHandler thing
#define IFMAP2C_IH_NAME(type)	type##Handler

#define IFMAP2C_IH_TOXML_DEF(type, parname)			\
XmlMarshalable *IFMAP2C_IH_NAME(type)::toXml(Identifier *const parname)

#define IFMAP2C_IH_FROMXML_DEF(type, parname)			\
type *IFMAP2C_IH_NAME(type)::fromXml(XmlMarshalable *const parname)

#define IFMAP2C_IH_TOXML_DECL(type, parname)			\
XmlMarshalable *toXml(Identifier *const parname)

#define IFMAP2C_IH_FROMXML_DECL(type, parname)			\
type *fromXml(XmlMarshalable *const parname)

#define IFMAP2C_IH_CANHANDLE_DEF(type, parname)			\
bool canHandle(Identifier * const parname) const {		\
	return typeid(*(parname)) == typeid(type);		\
}

#define IFMAP2C_IH_HEADER(type)					\
class IFMAP2C_IH_NAME(type) {					\
public:								\
	IFMAP2C_IH_TOXML_DECL(type, param);			\
	IFMAP2C_IH_FROMXML_DECL(type, param);			\
	IFMAP2C_IH_CANHANDLE_DEF(type, param);			\
};

} // namespace
#endif /* IDENTIFIER_H_ */
