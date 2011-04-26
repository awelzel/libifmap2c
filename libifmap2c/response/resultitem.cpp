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

#include <typeinfo>
#include "resultitem.h"
#include "typedefs.h"


namespace ifmap2c {

ResultItem::ResultItem() : numberIdentifiers(0),
		_i1(NULL),
		_i2(NULL)
{ }



ResultItem::~ResultItem()
{
	if (_i1)
		delete _i1;

	if (_i2)
		delete _i2;

	// delete all metadata childs
	XMLMLISTIT it = _metadataList.begin();
	XMLMLISTIT end = _metadataList.end();
	XmlMarshalable *child;
	for (/* see above */; it != end; it++) {
		child = *it;
		delete child;
	}
	_metadataList.clear();
}



void
ResultItem::ResultItem::setIpAddress(IpAddress *const ip)
{
	checkAndIncIdentifierNumber();

	if (_i1) {
		_i2 = ip;
	} else {
		_i1 = ip;
	}
}

void
ResultItem::ResultItem::setMacAddress(MacAddress *const mac)
{
	checkAndIncIdentifierNumber();

	if (_i1) {
		_i2 = mac;
	} else {
		_i1 = mac;
	}
}

void
ResultItem::ResultItem::setDevice(Device *const dev)
{
	checkAndIncIdentifierNumber();

	if (_i1) {
		_i2 = dev;
	} else {
		_i1 = dev;
	}
}

void
ResultItem::ResultItem::setAccessRequest(AccessRequest *const ar)
{
	checkAndIncIdentifierNumber();

	if (_i1) {
		_i2 = ar;
	} else {
		_i1 = ar;
	}
}

void
ResultItem::ResultItem::setIdentity(Identity *const id)
{
	checkAndIncIdentifierNumber();

	if (_i1) {
		_i2 = id;
	} else {
		_i1 = id;
	}
}



IpAddress *
ResultItem::getIpAddress(void) const
{
	if (_i1)
		if (typeid(*_i1) == typeid(IpAddress))
			return (IpAddress *)_i1;

	if (_i2)
		if (typeid(*_i2) == typeid(IpAddress))
			return (IpAddress *)_i2;
	return NULL;
}



MacAddress *
ResultItem::getMacAddress(void) const
{
	if (_i1)
		if (typeid(*_i1) == typeid(MacAddress))
			return (MacAddress *)_i1;

	if (_i2)
		if (typeid(*_i2) == typeid(MacAddress))
			return (MacAddress *)_i2;
	return NULL;
}



Device *
ResultItem::getDevice(void) const
{
	if (_i1)
		if (typeid(*_i1) == typeid(Device))
			return (Device *)_i1;

	if (_i2)
		if (typeid(*_i2) == typeid(Device))
			return (Device *)_i2;
	return NULL;
}



AccessRequest *
ResultItem::getAccessRequest() const
{
	if (_i1)
		if (typeid(*_i1) == typeid(AccessRequest))
			return (AccessRequest *)_i1;

	if (_i2)
		if (typeid(*_i2) == typeid(AccessRequest))
			return (AccessRequest *)_i2;
	return NULL;
}



Identity *
ResultItem::getIdentity(void) const
{
	if (_i1)
		if (typeid(*_i1) == typeid(Identity))
			return (Identity *)_i1;

	if (_i2)
		if (typeid(*_i2) == typeid(Identity))
			return (Identity *)_i2;
	return NULL;
}


Identifier *
ResultItem::getIdentifier1(void) const
{
	return _i1;
}


Identifier *
ResultItem::getIdentifier2(void) const
{
	return _i2;
}


void
ResultItem::addMetadata(XmlMarshalable *const md)
{
	_metadataList.push_back(md);
}



const CXMLMLIST&
ResultItem::getMetadataList(void) const
{
	return _metadataList;
}

void
ResultItem::checkAndIncIdentifierNumber(void)
{
	if (numberIdentifiers > 1)
		throw ResultItemError("Too many identifiers in ResultItem");
}

} // namespace
