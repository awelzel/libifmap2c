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

#ifndef IDENTIFIERS_H_
#define IDENTIFIERS_H_

#include "identifier.h"
#include "device.h"
#include "ipaddress.h"
#include "macaddress.h"
#include "accessrequest.h"
#include "identity.h"

namespace ifmap2c {

/*
 * A factory class to create all kinds of identifiers.
 */
class Identifiers {

public:
	static AccessRequest *createAr(const std::string& name,
			const std::string ad = "");

	static IpAddress *createIPv4(const std::string& value,
			const std::string ad = "");

	static IpAddress *createIPv6(const std::string& value,
			const std::string ad = "");

	static MacAddress *createMac(const std::string& value,
			const std::string ad = "");

	static Device *createDevAik(const std::string& aik);

	static Device *createDevName(const std::string& name);

	static Identity *createIdentity(IdentityType type,
			const std::string& name,
			const std::string& ad = "");

	static Identity *createOtherIdentity(const std::string& otherTypeDef,
			const std::string& name,
			const std::string& ad = "");

	static bool same(Identifier *const i1, Identifier *const i2);
	
	static bool sameType(Identifier *const i1, Identifier *const i2);

private:

	static bool sameAd(IdentifierWithAD *const i1, IdentifierWithAD *const i2);

	static bool sameIp(IpAddress *const ip1, IpAddress *const ip2);

	static bool sameMac(MacAddress *const mac1, MacAddress *const mac2);

	static bool sameDev(Device const *const dev1, Device *const dev2);

	static bool sameAr(AccessRequest *const ar1, AccessRequest *const ar2);

	static bool sameIdentity(Identity *const id1, Identity *const id2);


};

} // namespace
#endif /* IDENTIFIERS_H_ */
