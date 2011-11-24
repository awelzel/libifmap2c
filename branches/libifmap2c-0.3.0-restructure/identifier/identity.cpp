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
	"username",
	"sip-uri",
	"tel-uri",
	"other"
};

static const map<string, IdentityType> loadTypes(void)
{
	map<string, IdentityType> ret;
	ret[Identity::identityTypeNames[aik_name]] = aik_name;
	ret[Identity::identityTypeNames[distinguished_name]] 
		= distinguished_name;
	ret[Identity::identityTypeNames[dns_name]] = dns_name;
	ret[Identity::identityTypeNames[email_address]] = email_address;
	ret[Identity::identityTypeNames[hip_hit]] = hip_hit;
	ret[Identity::identityTypeNames[kerberos_principal]] 
		= kerberos_principal;
	ret[Identity::identityTypeNames[username]] = username;
	ret[Identity::identityTypeNames[sip_uri]] = sip_uri;
	ret[Identity::identityTypeNames[tel_uri]] = tel_uri;
	ret[Identity::identityTypeNames[other]] = other;
	return ret;
}

const map<string, IdentityType> Identity::identityTypes(loadTypes());

Identity::Identity(IdentityType type, const string& name, const string& ad,
	const string& ot) : 
	IdentifierAdmin(ad),
	_name(name),
	_otherDef(ot),
	_identityType(type)
{ }

IdentityType
Identity::getIdentityType() const
{
	return _identityType;
}

const std::string&
Identity::getTypeString() const
{
	return identityTypeNames[_identityType];
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

Identity *
Identity::clone(void) const
{
	return new Identity(getIdentityType(), getName(),
			getAdministrativeDomain(),
			getOtherTypeDef());
}


} // namespace
