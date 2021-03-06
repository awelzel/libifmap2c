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

#include "basicxmlmarshalable.h"

#include "xmlmarshalable.h"
#include "typedefs.h"

#include <iostream>

using namespace std;

namespace ifmap2c {

BasicXmlMarshalable::BasicXmlMarshalable(
				const string& name,
				const string& val,
				const pair<string, string>& n) :
	  _xmlElementName(name)
	, _xmlElementValue(val)
	, _xmlNamespace(n)
{ }
BasicXmlMarshalable::~BasicXmlMarshalable()
{
	_xmlNamespaceDefinitions.clear();
	_xmlAttributes.clear();

	/* delete all childs */
	XMLMLISTIT it = _xmlChildren.begin();
	XMLMLISTIT end = _xmlChildren.end();

	for (/* see above */; it != end; it++)
		delete *it;

	_xmlChildren.clear();
}



const string&
BasicXmlMarshalable::getXmlElementName(void) const
{
	return _xmlElementName;
}



const string&
BasicXmlMarshalable::getXmlElementValue(void) const
{
	return _xmlElementValue;
}



const STRP&
BasicXmlMarshalable::getXmlNamespace(void) const
{
	return _xmlNamespace;
}



const STRPLIST&
BasicXmlMarshalable::getXmlNamespaceDefinitions(void) const
{
	return _xmlNamespaceDefinitions;
}



const STRPLIST&
BasicXmlMarshalable::getXmlAttributes(void) const
{
	return _xmlAttributes;
}



const XMLMLIST&
BasicXmlMarshalable::getXmlChildren(void) const
{
	return _xmlChildren;
}



void
BasicXmlMarshalable::addXmlAttribute(const STRP& attr)
{
	_xmlAttributes.push_back(attr);
}

void
BasicXmlMarshalable::clearXmlAttributes(void)
{
	_xmlAttributes.clear();
}



void
BasicXmlMarshalable::addXmlChild(XmlMarshalable *const child)
{
	_xmlChildren.push_back(child);
}

void
BasicXmlMarshalable::clearXmlChildren(void)
{
	_xmlChildren.clear();
}



void
BasicXmlMarshalable::addXmlNamespaceDefinition(const STRP& ns)
{
	_xmlNamespaceDefinitions.push_back(ns);
}

XmlMarshalable *
BasicXmlMarshalable::clone(void) const
{
	BasicXmlMarshalable *ret = new BasicXmlMarshalable(
			getXmlElementName(),
			getXmlElementValue(),
			getXmlNamespace());

	CXMLMLISTIT cit = getXmlChildren().begin();
	CXMLMLISTIT cend = getXmlChildren().end();
	XmlMarshalable *child = NULL;

	// clone all the children
	for (/* see above */; cit != cend; cit++) {
		child = *cit;
		ret->addXmlChild(child->clone());
	}

	CSTRPLISTIT sit = getXmlAttributes().begin();
	CSTRPLISTIT send = getXmlAttributes().end();
	STRP strp;

	// copy all attributes
	for (/* see above */; sit != send; sit++) {
		strp = *sit;
		ret->addXmlAttribute(strp);
	}

	sit = getXmlNamespaceDefinitions().begin();
	send = getXmlNamespaceDefinitions().end();

	// copy all namespace definitions
	for (/* see above*/;  sit != send; sit++) {
		strp = *sit;
		ret->addXmlNamespaceDefinition(strp);
	}

	return ret;
}

} // namespace
