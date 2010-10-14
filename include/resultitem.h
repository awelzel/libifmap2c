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

#ifndef RESULTITEM_H_
#define RESULTITEM_H_
#include "xmlmarshalable.h"
#include "identifiers.h"

#include <list>

namespace ifmap2c {

class ResultItemError {
public:
	ResultItemError(const std::string& msg) : message(msg) { };

	const std::string& getMessage() { return message; }

private:
	std::string message;
};

class ResultItem {

public:
	ResultItem();

	virtual ~ResultItem();

	void setIpAddress(IpAddress *const ip);
	void setMacAddress(MacAddress *const mac);
	void setDevice(Device *const dev);
	void setAccessRequest(AccessRequest *const ar);
	void setIdentity(Identity *const id);

	IpAddress *getIpAddress(void) const;
	MacAddress *getMacAddress(void) const;
	Device *getDevice(void) const;
	AccessRequest *getAccessRequest(void) const;
	Identity *getIdentity(void) const;

	void addMetadata(XmlMarshalable *const md);

	const std::list<XmlMarshalable *>& getMetadataList(void) const;


private:
	std::list<XmlMarshalable *> _metadataList;
	int numberIdentifiers;

	IpAddress *_ip;
	MacAddress *_mac;
	Device *_dev;
	AccessRequest *_ar;
	Identity *_id;

	void checkAndIncIdentifierNumber(void);
};

} // namespace
#endif /* RESULTITEM_H_ */
