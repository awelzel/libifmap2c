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

#include "ifmap/ifmapbaseidentifierhandler.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"

#include <string>

using namespace std;
namespace ifmap2c {

/* FIXME: Helpers... */
static ifmap2c::XmlMarshalable *getFor(const string &name, const string &val = "")
{
	return new BasicXmlMarshalable(
			name,
			val,
			IDENTIFIER_NSPAIR);
}

static void handleAd(ifmap2c::XmlMarshalable *ret, ifmap2c::IdentifierAdmin *id)
{
	if (id->getAdministrativeDomain().length() > 0)
		ret->addXmlAttribute(STRP(ADMIN_DOMAIN_ATTR_NAME,
			id->getAdministrativeDomain()));
}

static bool cmpName(XmlMarshalable *const el, const string& name,
		const string& href)
{
	return XmlMarshalable::compNameNs(el, name, href);
}

static bool cmpAttrName(const STRP& attr, const string& attrname)
{
	return !attr.first.compare(attrname);
}


IFMAP2C_IH_TOXML_DEF(AccessRequest, id)
{
	AccessRequest *ar = dynamic_cast<AccessRequest *>(id);
	if (!ar) throw "UH AR";

	XmlMarshalable *ret = getFor(ACCESSREQ_ELEMENT_NAME);
	handleAd(ret, ar);

	ret->addXmlAttribute(
		STRP(ACCESSREQ_NAME_ATTR_NAME, ar->getName()));
	
	return ret;
}

IFMAP2C_IH_TOXML_DEF(Device, id)
{
	Device *dev = dynamic_cast<Device *>(id);
	if (!dev) throw "UH DEV";

	XmlMarshalable *ret = getFor(DEVICE_ELEMENT_NAME);
	ret->addXmlChild(getFor(DEVICE_NAME_ELEMENT, dev->getValue()));

	return ret;
}

IFMAP2C_IH_TOXML_DEF(Identity, ix) {
	Identity *id = dynamic_cast<Identity *>(ix);
	if (!id) throw "UH ID";

	XmlMarshalable *ret = getFor(IDENTITY_ELEMENT_NAME);
	handleAd(ret, id);
	ret->addXmlAttribute(STRP(IDENTITY_NAME_ATTR_NAME,
				id->getName()));
	ret->addXmlAttribute(STRP(IDENTITY_TYPE_ATTR_NAME,
				id->getTypeString()));

	if (id->getIdentityType() == other)
		ret->addXmlAttribute(STRP(IDENTITY_OTHER_DEF_ATTR_NAME,
				id->getOtherTypeDef()));

	return ret;
}

IFMAP2C_IH_TOXML_DEF(IpAddress, id) {
	IpAddress *ip = dynamic_cast<IpAddress *>(id);
	if (!ip) throw "UH IP";

	XmlMarshalable *ret = getFor(IPADDR_ELEMENT_NAME);
	handleAd(ret, ip);
	ret->addXmlAttribute(STRP(IPADDR_VALUE_ATTR_NAME, ip->getValue()));
	
	if (ip->getIpAddressType() == ipv4) {
		ret->addXmlAttribute(STRP(IPADDR_TYPE_ATTR_NAME,
					IPADDR_TYPE_IPV4));
	} else if (ip->getIpAddressType() == ipv6) {
		ret->addXmlAttribute(STRP(IPADDR_TYPE_ATTR_NAME,
					IPADDR_TYPE_IPV6));
	} else {
		throw "UH unknown type :(";
	}

	return ret;
}

IFMAP2C_IH_TOXML_DEF(MacAddress, id) {
	MacAddress *mac = dynamic_cast<MacAddress *>(id);
	if (!mac) throw "UH MAC";

	XmlMarshalable *ret = getFor(MACADDR_ELEMENT_NAME);
	handleAd(ret, mac);
	ret->addXmlAttribute(STRP(MACADDR_VALUE_ATTR_NAME,
				mac->getValue()));
	return ret;
}

IFMAP2C_IH_FROMXML_DEF(AccessRequest, xml) {
	bool foundName = false;
	CSTRPLISTIT it, end;
	string name, ad;

	/* Bail out if not AccessRequest */
	if (!cmpName(xml, ACCESSREQ_ELEMENT_NAME, ACCESSREQ_ELEMENT_HREF)) 
		return NULL;

	it = xml->getXmlAttributes().begin();
	end = xml->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (cmpAttrName(*it, ACCESSREQ_NAME_ATTR_NAME)) {
			name = (*it).second;
			foundName = true;
		} else if (cmpAttrName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
			ad = (*it).second;
		}
	}

	if (!foundName)
		throw BadIdentifier("AccessRequest has no name");

	return AccessRequest::createAccessRequest(name, ad);
}

IFMAP2C_IH_FROMXML_DEF(Device, xml) {
	CXMLMLISTIT it;

	/* Bail out if not Device */
	if (!cmpName(xml, DEVICE_ELEMENT_NAME, DEVICE_ELEMENT_HREF))
		return NULL;

	if (xml->getXmlChildren().size() > 1)
		throw BadIdentifier("Device has multiple child elements");

	it = xml->getXmlChildren().begin();

	if (!cmpName(*it, DEVICE_NAME_ELEMENT, DEVICE_NAME_HREF))
		throw BadIdentifier("Device has no \"name\" element");

	return Device::createDevice((*it)->getXmlElementValue());
}

IFMAP2C_IH_FROMXML_DEF(Identity, xml) {
	bool foundType = false, foundName = false, foundOther = false;
	string typeStr, name, otherDefStr, ad;
	IdentityType type;
	CSTRPLISTIT it, end;
	map<string, IdentityType>::const_iterator typeIt;

	/* Bail out if not Identity */
	if (!cmpName(xml, IDENTITY_ELEMENT_NAME, IDENTITY_ELEMENT_HREF))
		return NULL;

	it = xml->getXmlAttributes().begin();
	end = xml->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (cmpAttrName(*it, IDENTITY_TYPE_ATTR_NAME)) {
			typeStr = (*it).second;
			foundType = true;
		} else if (cmpAttrName(*it, IDENTITY_NAME_ATTR_NAME)) {
			name = (*it).second;
			foundName = true;
		} else if (cmpAttrName(*it,IDENTITY_OTHER_DEF_ATTR_NAME)) {
			otherDefStr = (*it).second;
			foundOther = true;
		} else if (cmpAttrName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
			ad = (*it).second;
		}
	}
	
	if (!foundType)
		throw BadIdentifier("Identity has no type");

	if (!foundName)
		throw BadIdentifier("Identity has no name");

	typeIt = Identity::identityTypes.find(typeStr);

	if (typeIt == Identity::identityTypes.end())
		throw BadIdentifier("Identity has unknown type");

	type = (*typeIt).second;

	if (type == other && !foundOther)
		throw BadIdentifier("Identity with other type definition"
			" has no other-type-definition attribute");

	if (type != other)
		return Identity::createIdentity(type, name, ad);

	return Identity::createOtherIdentity(otherDefStr, name, ad);
}

IFMAP2C_IH_FROMXML_DEF(IpAddress, xml) {
	IpAddressType type = ipv4;
	bool foundValue = false;
	string value, ad;
	CSTRPLISTIT it, end;

	if (!cmpName(xml, IPADDR_ELEMENT_NAME, IPADDR_ELEMENT_HREF))
		return NULL;
	it = xml->getXmlAttributes().begin();
	end = xml->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (cmpAttrName(*it, IPADDR_VALUE_ATTR_NAME)) {
			value = (*it).second;
			foundValue = true;
		} else if (cmpAttrName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
			ad = (*it).second;
		} else if (cmpAttrName(*it, IPADDR_TYPE_ATTR_NAME)) {
			if (!(*it).second.compare(IPADDR_TYPE_IPV6))
				type = ipv6;
		}
	}

	if (!foundValue)
		throw BadIdentifier("IpAddress has no value");

	if (type != ipv4)
		return IpAddress::createIpv6Address(value, ad);

	return IpAddress::createIpv4Address(value, ad);
}

IFMAP2C_IH_FROMXML_DEF(MacAddress, xml) {
	bool valueFound = false;
	string value, ad;
	CSTRPLISTIT it, end;

	if (!cmpName(xml, MACADDR_ELEMENT_NAME, MACADDR_ELEMENT_HREF))
		return NULL;

	it = xml->getXmlAttributes().begin();
	end = xml->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (cmpAttrName(*it, MACADDR_VALUE_ATTR_NAME)) {
			value = (*it).second;
			valueFound = true;
		} else if (cmpAttrName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
			ad = (*it).second;
		}
	}

	if (!valueFound)
		throw BadIdentifier("MacAddress has no value");

	return MacAddress::createMacAddress(value, ad);
}

} // namespace
