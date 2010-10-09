/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2010, A. Welzel, <arne.welzel@gmail.com>
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

namespace LIBRARY_NAMESPACE {

class Metadata {


public:
	static XmlMarshalable *createArDev();
		
	static XmlMarshalable *createArIp();

	static XmlMarshalable *createArMac();

	static XmlMarshalable *createAuthAs();
		
	static XmlMarshalable *createAuthBy();
		
	static XmlMarshalable *createDevIp();
		
	static XmlMarshalable * createIpMac(const std::string& start = "",
			const std::string& end = "",
			const std::string& dhcp = "");

	static XmlMarshalable *createLayer2Info(const char *const vlan,
			const char *const vlanName,
			const char *const port,
			const char *const admDomain);

	static std::string cardinalityNames[];

private:
	static XmlMarshalable *
	createMetadata(const std::string& name, CardinalityType card);
};

} // namespace
#endif /* METADATAFACTORY_H */
