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

#include "libxml2unmarshaller.h"
#include "typedefs.h"
#include <iostream>

#define CCHARP (const char *)

using namespace std;

namespace LIBRARY_NAMESPACE {

XmlMarshalable *
LibXml2Unmarshaller::unmarshal(const Payload& p)
{
	xmlDocPtr doc = xmlParseMemory(p.memory, p.length);
	
	if (doc == NULL)
		throw XmlUnmarshalError("Could not parse document");

	xmlNodePtr root = xmlDocGetRootElement(doc);

	if (root == NULL)
		throw XmlUnmarshalError("Could not get root element");
	
	XmlMarshalable *ret = unmarshalXmlNode(root);

	xmlFreeDoc(doc);

	return ret;
}



XmlMarshalable *
LibXml2Unmarshaller::unmarshalXmlNode(xmlNodePtr node)
{
	string name(CCHARP node->name);
	string val("");
	STRP ns("","");

	for (xmlNodePtr child = node->children; child != NULL; child = child->next) {
		if (child->type == XML_TEXT_NODE) {
			val = CCHARP child->content;
		}
	}

	if (node->ns != NULL) {
		ns = STRP(CCHARP node->ns->prefix, CCHARP node->ns->href);
	}

	XmlMarshalable *ret = new BasicXmlMarshalable(name, val, ns);

	for (xmlNsPtr ns = node->nsDef; ns != NULL; ns = ns->next) {
		ret->addXmlNamespaceDefinition(STRP(CCHARP ns->prefix, CCHARP ns->href));
	}

	for (xmlNodePtr child = node->children; child != NULL; child = child->next) {
		if (child->type == XML_ELEMENT_NODE)
			ret->addXmlChild(unmarshalXmlNode(child));
	}

	for (xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next) {
		ret->addXmlAttribute(STRP(CCHARP attr->name, CCHARP attr->children->content));
	}

	return ret;
}



XmlUnmarshaller *
LibXml2Unmarshaller::createUnmarshaller()
{
	return new LibXml2Unmarshaller();
}

} // namespace
