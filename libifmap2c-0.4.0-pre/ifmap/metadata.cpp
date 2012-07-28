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

#include "metadata.h"
#include "basicxmlmarshalable.h"
#include "tcgifmapbase.h"
#include "typedefs.h"

using namespace std;

namespace ifmap2c {

string Metadata::cardinalityNames[] = {
	"singleValue",
	"multiValue"
};

XmlMarshalable *
Metadata::createMetadata(const string& name, CardinalityType card)
{
	XmlMarshalable *ret = new BasicXmlMarshalable(name, EMPTY_VALUE, TCG_META_NSPAIR);
	ret->addXmlAttribute(STRP(META_CARDINALITY_ATTR_NAME, cardinalityNames[card]));
	return ret;
}



XmlMarshalable *
Metadata::createArDev(void)
{
	return createMetadata(META_ARDEV_ELEMENT_NAME, META_ARDEV_CARDINALITY);
}


XmlMarshalable *
Metadata::createArIp(void)
{
	return createMetadata(META_ARIP_ELEMENT_NAME, META_ARIP_CARDINALITY);
}
		
XmlMarshalable *
Metadata::createArMac(void)
{
	return createMetadata(META_ARMAC_ELEMENT_NAME, META_ARMAC_CARDINALITY);
}



XmlMarshalable *
Metadata::createAuthAs(void)
{
	return createMetadata(META_AUTHAS_ELEMENT_NAME, META_AUTHAS_CARDINALITY);
}
	
XmlMarshalable *
Metadata::createAuthBy(void)
{
	return createMetadata(META_AUTHBY_ELEMENT_NAME, META_AUTHBY_CARDINALITY);
}
		


XmlMarshalable *
Metadata::createDevIp(void)
{
	return createMetadata(META_DEVIP_ELEMENT_NAME, META_DEVIP_CARDINALITY);
}

XmlMarshalable *
Metadata::createRole(const string& name, const string& admDomain)
{
	XmlMarshalable *ret = createMetadata(META_ROLE_ELEMENT_NAME,
			META_ROLE_CARDINALITY);

	if (admDomain.length() > 0)
		ret->addXmlChild(new BasicXmlMarshalable(META_ROLE_ADMD_ELEMENT_NAME,
				admDomain, NO_NSPAIR));

	ret->addXmlChild(new BasicXmlMarshalable(META_ROLE_NAME_ELEMENT_NAME,
			name, NO_NSPAIR));

	return ret;
}



XmlMarshalable *
Metadata::createIpMac(const string& start, const string& end,
		const string& dhcp)
{
	XmlMarshalable *elem, *child;
	elem = createMetadata(META_IPMAC_ELEMENT_NAME, META_IPMAC_CARDINALITY);

	if (start.length() > 0) {
		child = new BasicXmlMarshalable(META_IPMAC_START_TIME,
				start, NO_NSPAIR);
		elem->addXmlChild(child);
	}

	if (end.length() > 0) {
		child = new BasicXmlMarshalable(META_IPMAC_END_TIME,
				end, NO_NSPAIR);
		elem->addXmlChild(child);
	}

	if (dhcp.length() > 0) {
		child = new BasicXmlMarshalable(META_IPMAC_DHCP_SERVER,
				dhcp, NO_NSPAIR);
		elem->addXmlChild(child);
	}
	return elem;
}


XmlMarshalable *
Metadata::createLayer2Info(const char *const vlan,
		const char *const vlanName,
		const char *const port,
		const char *const admDomain)
{
	XmlMarshalable *ret = createMetadata(META_LAYER2INFO_ELEMENT_NAME,
			META_LAYER2INFO_CARDINALITY);
	
	if (vlan)
		ret->addXmlChild(new BasicXmlMarshalable(META_LAYER2INFO_VLAN_ELEMENT_NAME,
				vlan, NO_NSPAIR));

	if (vlanName)
		ret->addXmlChild(new BasicXmlMarshalable(META_LAYER2INFO_VLANNAME_ELEMENT_NAME,
				vlanName, NO_NSPAIR));

	if (port)
		ret->addXmlChild(new BasicXmlMarshalable(META_LAYER2INFO_PORT_ELEMENT_NAME,
				port, NO_NSPAIR));

	if (admDomain)
		ret->addXmlChild(new BasicXmlMarshalable(META_LAYER2INFO_ADMINDOM_ELEMENT_NAME,
				admDomain, NO_NSPAIR));

	return ret;
}

XmlMarshalable *
Metadata::createCapability(const string& name, const string& ad)
{
	XmlMarshalable *ret = createMetadata(META_CAPABILITY_ELEMENT_NAME,
			META_CAPABILITY_CARDINALITY);

	ret->addXmlChild(new BasicXmlMarshalable(META_CAPABILITY_NAME_ELEMENT_NAME,
			name, NO_NSPAIR));

	if (ad.length() > 0) {
		ret->addXmlChild(new BasicXmlMarshalable(META_CAPABILITY_ADMD_ELEMENT_NAME,
			ad, NO_NSPAIR));
	}

	return ret;
}

XmlMarshalable *
Metadata::createDevAttr(const string& name)
{
	XmlMarshalable *ret = createMetadata(META_DEV_ATTR_ELEMENT_NAME,
			META_DEV_ATTR_CARDINALITY);

	ret->addXmlChild(new BasicXmlMarshalable(META_DEV_ATTR_NAME_ELEMENT_NAME,
			name, NO_NSPAIR));
	return ret;
}

XmlMarshalable *
Metadata::createDevChar(const string& time, const string& method, const string& id,
					char const *manufacturer,
					char const *model,
					char const *os,
					char const *osvers,
					char const *devtype)
{
	XmlMarshalable *ret = createMetadata(META_DEV_CHAR_ELEMENT_NAME,
			META_DEV_CHAR_CARDINALITY);

	if (manufacturer)
		ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_MANU_ELEMENT_NAME,
			manufacturer, NO_NSPAIR));

	if (model)
		ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_MODEL_ELEMENT_NAME,
			model, NO_NSPAIR));
	if (os)
		ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_OS_ELEMENT_NAME,
			os, NO_NSPAIR));
	if (osvers)
		ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_OSVER_ELEMENT_NAME,
			osvers, NO_NSPAIR));

	if (devtype)
		ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_DEVTYPE_ELEMENT_NAME,
			devtype, NO_NSPAIR));

	ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_DISC_TIME_ELEMENT_NAME,
			time, NO_NSPAIR));
	ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_DISC_ID_ELEMENT_NAME,
			id, NO_NSPAIR));
	ret->addXmlChild(new BasicXmlMarshalable(META_DEV_CHAR_DISC_METH_ELEMENT_NAME,
			method, NO_NSPAIR));

	return ret;
}

XmlMarshalable *
Metadata::createDiscoveredBy(void)
{
	return createMetadata(META_DISCBY_ELEMENT_NAME, META_DISCBY_CARDINALITY);
}

UserMetadata::UserMetadata(const string& elName,
			const string &prefix,
			const string &href,
			CardinalityType card,
			const string &elValue) :
	BasicXmlMarshalable(elName, elValue, STRP(prefix, href))
{
	STRP cardattr = STRP(META_CARDINALITY_ATTR_NAME,
			Metadata::cardinalityNames[card]);
	addXmlAttribute(cardattr);
}

UserMetadata::UserMetadata(const string& elName,
			const string &prefix,
			const string &href,
			const string &elValue) :
	BasicXmlMarshalable(elName, elValue, STRP(prefix, href))
{
	STRP cardattr = STRP(META_CARDINALITY_ATTR_NAME,
			Metadata::cardinalityNames[single]);
	addXmlAttribute(cardattr);
}

} // namespace
