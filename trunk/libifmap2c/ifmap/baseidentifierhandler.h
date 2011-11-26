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

#ifndef BASEIDENTIFIERHANDLER_H_
#define BASEIDENTIFIERHANDLER_H_
#include "identifier.h"
#include "identifiers.h"

// Some macros to safe some typing for the IdentifierHandler thing
#define IFMAP2C_IH_NAME(type)	type ## Handler

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

#define IFMAP2C_IH_CREATE_F	create

#define IFMAP2C_IH_CREATE_CALL(type)				\
	IFMAP2C_IH_NAME(type)::IFMAP2C_IH_CREATE_F()

#define IFMAP2C_IH_CREATE_DEF(type)				\
static IFMAP2C_IH_NAME(type) *					\
IFMAP2C_IH_CREATE_F(void)					\
{								\
	return new IFMAP2C_IH_NAME(type)();			\
}

#define IFMAP2C_IH_HEADER(type)					\
class IFMAP2C_IH_NAME(type) : public ifmap2c::IdentifierHandler {\
public:								\
	IFMAP2C_IH_TOXML_DECL(type, param);			\
	IFMAP2C_IH_FROMXML_DECL(type, param);			\
	IFMAP2C_IH_CANHANDLE_DEF(type, param);			\
	IFMAP2C_IH_CREATE_DEF(type);				\
								\
private:							\
	IFMAP2C_IH_NAME(type)() { };				\
};

namespace ifmap2c {

	IFMAP2C_IH_HEADER(AccessRequest);

	IFMAP2C_IH_HEADER(Device);

	IFMAP2C_IH_HEADER(Identity);

	IFMAP2C_IH_HEADER(IpAddress);

	IFMAP2C_IH_HEADER(MacAddress);

} // namespace

#endif /* BASEIDENTIFIERHANDLER_H_ */
