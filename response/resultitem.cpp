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

#include "resultitem.h"
#include "typedefs.h"

namespace ifmap2c {

ResultItem::ResultItem() : numberIdentifiers(0),
		_ip(NULL),
		_mac(NULL),
		_dev(NULL),
		_ar(NULL),
		_id(NULL)
{ }



ResultItem::~ResultItem()
{
	if (_ip)
		delete _ip;

	if (_mac)
		delete _mac;

	if (_dev)
		delete _dev;

	if (_ar)
		delete _ar;

	if (_id)
		delete _id;

	// delete all metadata childs
	XMLMLISTIT it = _metadataList.begin();
	XMLMLISTIT end = _metadataList.end();
	XmlMarshalable *child = NULL;
	for (child = *it; it != end; child = *(++it))
		delete child;
	_metadataList.clear();
}



void
ResultItem::ResultItem::setIpAddress(IpAddress *const ip)
{
	checkAndIncIdentifierNumber();

	if (!_ip)
		_ip = ip;
	else
		throw ResultItemError("Two IpAddress in ResultItem");
}

void
ResultItem::ResultItem::setMacAddress(MacAddress *const mac)
{
	checkAndIncIdentifierNumber();

	if (!_mac)
		_mac = mac;
	else
		throw ResultItemError("Two MacAddress in ResultItem");
}

void
ResultItem::ResultItem::setDevice(Device *const dev)
{
	checkAndIncIdentifierNumber();

	if (!_dev)
		_dev = dev;
	else
		throw ResultItemError("Two Device in ResultItem");
}

void
ResultItem::ResultItem::setAccessRequest(AccessRequest *const ar)
{
	checkAndIncIdentifierNumber();

	if (!_ar)
		_ar = ar;
	else
		throw ResultItemError("Two AccessRequest in ResultItem");
}

void
ResultItem::ResultItem::setIdentity(Identity *const id)
{
	checkAndIncIdentifierNumber();

	if (!_id)
		_id = id;
	else
		throw ResultItemError("Two Identity in ResultItem");
}



IpAddress *
ResultItem::getIpAddress(void) const
{
	return _ip;
}



MacAddress *
ResultItem::getMacAddress(void) const
{
	return _mac;
}



Device *
ResultItem::getDevice(void) const
{
	return _dev;
}



AccessRequest *
ResultItem::getAccessRequest() const
{
	return _ar;
}



Identity *
ResultItem::getIdentity(void) const
{
	return _id;
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
