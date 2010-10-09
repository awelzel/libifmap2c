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

#include "ipaddress.h"

#include "typedefs.h"
#include "tcgifmapbase.h"

using namespace std;

namespace LIBRARY_NAMESPACE {

IpAddress::IpAddress(IpAddressType type, const string& val, const string& ad) :
	IdentifierWithAD(IPADDR_ELEMENT_NAME, EMPTY_VALUE, ad),
	_value(val), _type(type)
{
	addXmlAttribute(STRP(IPADDR_VALUE_ATTR_NAME, val));
	
	if (type == ipv4) {
		addXmlAttribute(STRP(IPADDR_TYPE_ATTR_NAME, IPADDR_TYPE_IPV4));
	} else if (type == ipv6) {
		addXmlAttribute(STRP(IPADDR_TYPE_ATTR_NAME, IPADDR_TYPE_IPV6));
	}

}

const string&
IpAddress::getValue() const
{
	return _value;
}

IpAddressType
IpAddress::getType() const
{
	return _type;
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

} // namespace
