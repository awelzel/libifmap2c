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

#include "identifiers.h"
#include <typeinfo>
#include <string>

using namespace std;

namespace ifmap2c {

Identifier::~Identifier() { }

IdentifierAdmin::IdentifierAdmin(const string& ad) :
	_administrativeDomain(ad) { }


IdentifierAdmin::~IdentifierAdmin() { }

const std::string&
IdentifierAdmin::getAdministrativeDomain(void) const
{
	return _administrativeDomain;
}

IdentifierHolder::~IdentifierHolder()
{
	if (_i1)
		delete _i1;

	if (_i2)
		delete _i2;
}

IdentifierHolder::IdentifierHolder(Identifier *const i1, 
	Identifier *const i2) :
		_i1(i1), _i2(i2)
{ }

Identifier *
IdentifierHolder::getIdentifier1(void) const {
	return _i1;
}
	
Identifier *
IdentifierHolder::getIdentifier2(void) const {
	return _i2;
}

IdentifierMetadataHolder::IdentifierMetadataHolder(Identifier *const i1,
		Identifier *const i2, std::list<XmlMarshalable *> mList)
	: IdentifierHolder(i1, i2),
	_metadata(mList)
{ }

IdentifierMetadataHolder::~IdentifierMetadataHolder()
{
	std::list<XmlMarshalable *>::const_iterator it, end;
	it = _metadata.begin();
	end = _metadata.end();

	for (/* */; it != end; it++)
		delete *it;
}

const std::list<XmlMarshalable *>&
IdentifierMetadataHolder::getMetadata(void) const {
	return _metadata;
}

AccessRequest::AccessRequest(const string& name, const string& ad) :
	IdentifierAdmin(ad), _name(name) { }

AccessRequest *
AccessRequest::createAccessRequest(const string& name, const string& ad)
{
	return new AccessRequest(name, ad);
}

const std::string&
AccessRequest::getName() const
{
	return _name;
}

AccessRequest *AccessRequest::clone(void) const
{
	return new AccessRequest(getName(), getAdministrativeDomain());
}

Device::Device(const string& val) : _value(val) { }

Device *
Device::createDevice(const string& val)
{
	return new Device(val);
}

const string&
Device::getValue() const
{
	return _value;
}

Device *
Device::clone(void) const
{
	return new Device(_value);
}

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

IpAddress::IpAddress(IpAddressType type, const string& val, const string& ad) :
	IdentifierAdmin(ad),
	_value(val),
	_ipAddrType(type)
{ }

const string&
IpAddress::getValue() const
{
	return _value;
}

IpAddressType
IpAddress::getIpAddressType() const
{
	return _ipAddrType;
}

IpAddress *
IpAddress::createIpv4Address(const string& val, const string& ad)
{
	return new IpAddress(ipv4, val, ad);
}
	
IpAddress *
IpAddress::createIpv6Address(const string& val, const string& ad)
{
	return new IpAddress(ipv6, val, ad);
}

IpAddress *
IpAddress::clone(void) const
{
	return new IpAddress(getIpAddressType(), getValue(),
			getAdministrativeDomain());
}

MacAddress::MacAddress(const string& val, const string& ad) :
	IdentifierAdmin(ad), _value(val)
{ }

MacAddress *MacAddress::createMacAddress(const string& val, const string& ad)
{
	return new MacAddress(val, ad);
}

const string&
MacAddress::getValue() const
{
	return _value;
}

MacAddress *
MacAddress::clone(void) const
{
	return new MacAddress(getValue(), getAdministrativeDomain());
}

/* * * ** * * */
/* * * ** * * */
/* * * ** * * */
/* * * ** * * */
AccessRequest *
Identifiers::createAr(const string& name, const string ad)
{
	return AccessRequest::createAccessRequest(name, ad);

}

IpAddress *
Identifiers::createIPv4(const string& value, const string ad)
{
	return IpAddress::createIpv4Address(value, ad);
}


IpAddress *
Identifiers::createIPv6(const string& value, const string ad)
{
	return IpAddress::createIpv6Address(value, ad);
}

MacAddress *
Identifiers::createMac(const string& value, const string ad)
{
	return MacAddress::createMacAddress(value, ad);
}

Device *
Identifiers::createDev(const string& name)
{
	return Device::createDevice(name);
}

Identity *
Identifiers::createIdentity(IdentityType type, const string& name,
		const string& ad)
{
	return Identity::createIdentity(type, name, ad);
}

Identity *
Identifiers::createOtherIdentity(const string& otherTypeDef,
			const string& name,
			const string& ad)
{
	return Identity::createOtherIdentity(otherTypeDef, name, ad);
}

bool
Identifiers::sameType(Identifier *const i1, Identifier *const i2)
{
	if (i1 == i2)
		return true;
	
	if (i1 && i2)
		if (typeid(*i1) == typeid(*i2))
			return true;
	
	return false;
}

bool
Identifiers::same(Identifier *const i1, Identifier *const i2)
{
	if (i1 == i2) {
		return true;
	}

	if (i1 && i2) {
		if (typeid(*i1) == typeid(*i2)) {

			if (typeid(*i1) == typeid(IpAddress)) {
				return Identifiers::sameIp((IpAddress *)i1, (IpAddress *)i2);
			} else if (typeid(*i1) == typeid(MacAddress)) {
				return Identifiers::sameMac((MacAddress *)i1, (MacAddress *)i2);
			} else if (typeid(*i1) == typeid(Device)) {
				return Identifiers::sameDev((Device *)i1, (Device *)i2);
			} else if (typeid(*i1) == typeid(AccessRequest)) {
				return Identifiers::sameAr((AccessRequest *)i1, (AccessRequest *)i2);
			} else if (typeid(*i1) == typeid(Identity)) {
				return Identifiers::sameIdentity((Identity *)i1, (Identity *)i2);
			}
		}
	}
	return false;
}

bool
Identifiers::sameAd(IdentifierAdmin *const i1, IdentifierAdmin *const i2)
{
	const string &ad1 = i1->getAdministrativeDomain();
	const string &ad2 = i2->getAdministrativeDomain();
	if (!ad1.compare(ad2)) {
		return true;
	}
	return false;
}

bool
Identifiers::sameIp(IpAddress *const ip1, IpAddress *const ip2)
{
	const string &val1 = ip1->getValue();
	const string &val2 = ip2->getValue();
		return sameAd(ip1, ip2)
				&& (ip1->getIpAddressType() == ip2->getIpAddressType())
				&& !(val1.compare(val2));
}

bool
Identifiers::sameMac(MacAddress *const mac1, MacAddress *const mac2)
{
	const string &val1 = mac1->getValue();
	const string &val2 = mac2->getValue();
		return sameAd(mac1, mac2)
				&& !(val1.compare(val2));
}

bool
Identifiers::sameDev(Device const *const dev1, Device *const dev2)
{
	const string &val1 = dev1->getValue();
	const string &val2 = dev2->getValue();
	return !(val1.compare(val2));

}

bool
Identifiers::sameAr(AccessRequest *const ar1, AccessRequest *const ar2)
{
	const string &val1 = ar1->getName();
	const string &val2 = ar2->getName();
		return sameAd(ar1, ar2)
				&& !(val1.compare(val2));
}

bool
Identifiers::sameIdentity(Identity *const id1, Identity *const id2)
{
	if (sameAd(id1, id2) && (id1->getIdentityType() == id2->getIdentityType())) {
		// check the other type def if of type other
		if (id1->getIdentityType() == other) {
			const string &othertypedef1 = id1->getOtherTypeDef();
			const string &othertypedef2 = id2->getOtherTypeDef();

			if (othertypedef1.compare(othertypedef2)) {
				return false;
			}
		}

		const string &val1 = id1->getName();
		const string &val2 = id2->getName();

		return !(val1.compare(val2));
	}
	return false;
}


} // namespace
