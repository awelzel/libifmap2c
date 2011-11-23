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

#include "identifier/stdhandler.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"
#include <string>


using namespace std;
namespace ifmap2c {

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
	return NULL;
}

IFMAP2C_IH_FROMXML_DEF(AccessRequest, xml) {
	XmlMarshalable *xx = xml;
	xx++;
	return NULL;
}

IFMAP2C_IH_FROMXML_DEF(Device, xml) {
	XmlMarshalable *xx = xml;
	xx++;
	return NULL;
}

IFMAP2C_IH_FROMXML_DEF(Identity, xml) {
	XmlMarshalable *xx = xml;
	xx++;
	return NULL;
}

IFMAP2C_IH_FROMXML_DEF(IpAddress, xml) {
	XmlMarshalable *xx = xml;
	xx++;
	return NULL;
}

IFMAP2C_IH_FROMXML_DEF(MacAddress, xml) {
	XmlMarshalable *xx = xml;
	xx++;
	return NULL;
}

}
