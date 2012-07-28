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

#ifndef METADATAFACTORY_H_
#define METADATAFACTORY_H_
#include "xmlmarshalable.h"
#include "tcgifmapmeta.h"
#include <string>

enum CardinalityType { single, multi };

namespace ifmap2c {

class Metadata {


public:
	static XmlMarshalable *createArDev(void);
		
	static XmlMarshalable *createArIp(void);

	static XmlMarshalable *createArMac(void);

	static XmlMarshalable *createAuthAs(void);
		
	static XmlMarshalable *createAuthBy(void);
		
	static XmlMarshalable *createDevIp(void);

	static XmlMarshalable *createRole(const std::string& name,
			const std::string& admDomain = "");
		
	static XmlMarshalable * createIpMac(const std::string& start = "",
			const std::string& end = "", const std::string& dhcp = "");

	static XmlMarshalable *createLayer2Info(const char *const vlan,
			const char *const vlanName, const char *const port,
			const char *const admDomain);

	static XmlMarshalable *createCapability(const std::string& name,
			const std::string& ad = "");

	static XmlMarshalable *createDevAttr(const std::string& name);

	/**
	 * Create device characteristic metadata
	 */
	static XmlMarshalable *createDevChar(const std::string& time,
			const std::string& id,
			const std::string& method,
			char const *manufacturer = NULL,
			char const *model = NULL,
			char const *os = NULL,
			char const *osvers = NULL,
			char const *devtype = NULL);

	static XmlMarshalable *createDiscoveredBy(void);


	static std::string cardinalityNames[];

private:
	static XmlMarshalable *
	createMetadata(const std::string& name, CardinalityType card);
};

} // namespace
#endif /* METADATAFACTORY_H */
