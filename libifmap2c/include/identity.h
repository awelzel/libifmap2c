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

#ifndef IDENTITY_H_
#define IDENTITY_H_
#include "identifier.h"
#include <map>

namespace ifmap2c {

enum IdentityType {
          aik_name,
          distinguished_name,
          dns_name,
          email_address,
          hip_hit,
          kerberos_principal,
          username,
          sip_uri,
          tel_uri,
          other
};


class Identity : public IdentifierAdmin {

public:
	static Identity * createIdentity(IdentityType type,
			const std::string& name,
			const std::string& ad = "");

	static Identity *createOtherIdentity(const std::string& otherDef,
			const std::string& name,
			const std::string& ad = "");

	IdentityType getIdentityType() const;

	const std::string& getTypeString() const;

	const std::string& getName() const;

	const std::string& getOtherTypeDef() const;

	Identity *clone(void) const;

	static const std::string identityTypeNames[];

	static const std::map<std::string, IdentityType> identityTypes;

private:

	Identity(IdentityType type,
			const std::string& val,
			const std::string& ad="",
			const std::string& other="");

	std::string _name;
	std::string _otherDef;
	IdentityType _identityType;
};

} // namespace
#endif /* IDENTITY_H_ */
