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

#include "identity.h"

#include "typedefs.h"
#include "tcgifmapbase.h"

using namespace std;

namespace ifmap2c {

const string Identity::identityTypeNames[] = {
	"aik-name",
	"distinguished-name",
	"dns-name",
	"email-address",
	"hip-hit",
	"kerberos-principal",
	"trusted-platform-module",
	"username",
	"sip-uri",
	"tel-uri",
	"other"
};

Identity::Identity(IdentityType type, const string& name , const string& ad,
		const string& ot) :
	IdentifierWithAD(IDENTITY_ELEMENT_NAME, EMPTY_VALUE, ad)
{
	addXmlAttribute(STRP(IDENTITY_NAME_ATTR_NAME, name));
	addXmlAttribute(STRP(IDENTITY_TYPE_ATTR_NAME, identityTypeNames[type]));
	if (type == other)
		addXmlAttribute(STRP(IDENTITY_OTHER_DEF_ATTR_NAME, ot));
}



IdentityType Identity::getType() const
{
	return _type;
}


const std::string&
Identity::getTypeString() const
{
	return identityTypeNames[_type];
}



const std::string&
Identity::getName() const
{
	return _name;
}



const std::string&
Identity::getOtherTypeDef() const
{
	return _otherDef;
}



Identity *
Identity::createIdentity(IdentityType type, const string &name, const string& ad)
{
	return new Identity(type, name, ad);
}



Identity *
Identity::createOtherIdentity(const string& otherDef, const string& name,
	const string& ad)
{
	return new Identity(other, name, ad, otherDef);
}

} // namespace
