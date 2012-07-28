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

#ifndef IPADDRESS_H_
#define IPADDRESS_H_

#include "identifierwithad.h"

namespace ifmap2c {

enum IpAddressType { ipv4, ipv6 };

class IpAddress : public IdentifierWithAD {

public:

	const std::string& getValue() const;

	IpAddressType getIpAddressType() const;

	static IpAddress * createIpv4Address(const std::string& val,
			const std::string& ad = "");

	static IpAddress *createIpv6Address(const std::string& val,
			const std::string& ad = "");

	IpAddress *clone(void) const;

private:

	IpAddress(IpAddressType type, const std::string& val,
		const std::string& ad);

	std::string _value;
	IpAddressType _ipAddrType;

	static IpAddress *createIpAddress(const std::string& val,
		const std::string& ad);

};

} // namespace
#endif /* IPADDRESS_H_ */
