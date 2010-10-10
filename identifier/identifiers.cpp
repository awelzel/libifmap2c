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

#include "identifiers.h"

namespace ifmap2c {

AccessRequest *
Identifiers::createAr(const std::string& name, const std::string ad)
{
	return AccessRequest::createAccessRequest(name, ad);

}

IpAddress *
Identifiers::createIPv4(const std::string& value, const std::string ad)
{
	return IpAddress::createIpv4Address(value, ad);
}


IpAddress *
Identifiers::createIPv6(const std::string& value, const std::string ad)
{
	return IpAddress::createIpv6Address(value, ad);
}

MacAddress *
Identifiers::createMac(const std::string& value, const std::string ad)
{
	return MacAddress::createMacAddress(value, ad);
}

Device *
Identifiers::createDevAik(const std::string& aik)
{
	return Device::createDeviceAik(aik);
}

Device *
Identifiers::createDevName(const std::string& name)
{
	return Device::createDeviceName(name);
}

Identity *
Identifiers::createIdentity(IdentityType type, const std::string& name,
		const std::string& ad)
{
	return Identity::createIdentity(type, name, ad);
}

Identity *
Identifiers::createOtherIdentity(const std::string& otherTypeDef,
			const std::string& name,
			const std::string& ad)
{
	return Identity::createOtherIdentity(otherTypeDef, name, ad);
}

} // namespace
