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

#include "metadata.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"

using namespace std;

namespace LIBRARY_NAMESPACE {

std::string Metadata::cardinalityNames[] = {
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
Metadata::createArDev()
{
	return createMetadata(META_ARDEV_ELEMENT_NAME, META_ARDEV_CARDINALITY);
}


XmlMarshalable *
Metadata::createArIp()
{
	return createMetadata(META_ARIP_ELEMENT_NAME, META_ARIP_CARDINALITY);
}
		
XmlMarshalable *
Metadata::createArMac()
{
	return createMetadata(META_ARMAC_ELEMENT_NAME, META_ARMAC_CARDINALITY);
}



XmlMarshalable *
Metadata::createAuthAs()
{
	return createMetadata(META_AUTHAS_ELEMENT_NAME, META_AUTHAS_CARDINALITY);
}
	
XmlMarshalable *
Metadata::createAuthBy()
{
	return createMetadata(META_AUTHBY_ELEMENT_NAME, META_AUTHBY_CARDINALITY);
}
		


XmlMarshalable *
Metadata::createDevIp()
{
	return createMetadata(META_DEVIP_ELEMENT_NAME, META_DEVIP_CARDINALITY);
}



XmlMarshalable *
Metadata::createIpMac(const std::string& start,
		const std::string& end,
		const std::string& dhcp)
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

} // namespace
