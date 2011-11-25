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

#include "xml/libxml2binding.h"
#include "typedefs.h"
#include <string>
#include <cstring>

using namespace std;

namespace ifmap2c {


Payload
LibXml2Marshaller::marshal(XmlMarshalable *const root)
{
	if (!root)
		throw XmlMarshalError("Root element not given");

	Payload ret(NULL, 0);

	xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");

	xmlNodePtr rootNode = marshalToXmlNode(root);
	
	if (!rootNode)
		throw XmlMarshalError("Could not create root node");

	xmlDocSetRootElement(doc, rootNode);
	
	xmlChar *tmp;

	xmlDocDumpMemory(doc, &tmp, &ret.length);


	// allocate new memory for the dumped document
	ret.memory = new char[ret.length];

	// copy the document into the new buffer
	memcpy(ret.memory, tmp, ret.length);

	// free libxml stuff
	xmlFree(tmp);
	xmlFreeDoc(doc);

	// error, but what about freeing? (FIXME)
	if (ret.length == 0)
		throw XmlMarshalError("Could not dump");

	// sanity check
	if (_nsDeclarationList.size() != 0) {
		throw new XmlMarshalError("nsDeclList size not zero, Programming error?");
	}
	
	return ret;
}



xmlNodePtr
LibXml2Marshaller::marshalToXmlNode(XmlMarshalable *node)
{

	if (!node)
		throw XmlMarshalError();

	string nodeName = node->getXmlElementName();

	xmlNodePtr xmlNode = xmlNewNode(NULL,
			BAD_CAST nodeName.c_str());

	STRPLIST attrList = node->getXmlAttributes();

	STRPLISTIT it = attrList.begin();
	STRPLISTIT end = attrList.end();
	
	/* create new property nodes for the new xmlNode object */
	for (/* see above */; it != end; it++) {
		xmlNewProp(xmlNode, BAD_CAST it->first.c_str(),
				BAD_CAST it->second.c_str());
	}

	STRPLIST nsDecls = node->getXmlNamespaceDefinitions();
	it = nsDecls.begin();
	end = nsDecls.end();

	/* add all namespace declarations to this node */
	for (/* */; it != end; it++) {
		xmlNsPtr newNs = xmlNewNs(xmlNode,
				BAD_CAST it->second.c_str(),
				BAD_CAST it->first.c_str());
		// add the declared namespace object to the list of all ns in
		// the document
		_nsDeclarationList.push_back(NSDECL(it->first, newNs));
	}

	string prefix = node->getXmlNamespace().first;

	// if a prefix is set then its length > 0 and we lookup it
	// up in our list, beginning from the end...
	//
	// Some improvement could be done here (FIXME)
	if (prefix.length() > 0) {
		NSDECLLISTIT rit = _nsDeclarationList.rbegin();
		NSDECLLISTIT rend = _nsDeclarationList.rend();

		for (/* see above */; rit != rend; rit++) {
			if (!rit->first.compare(prefix)) {
				xmlSetNs(xmlNode, rit->second);
				break;
			}
		}
		
		if (rit == rend) {
			string msg = "Undeclared namespace: ";
			msg.append(prefix);
			throw XmlMarshalError(msg);
		}
	} else {
		// no namespace
	}

	XMLMLIST childs = node->getXmlChildren();
	XMLMLISTIT cit = childs.begin();
	XMLMLISTIT cend = childs.end();

	/* do the whole thing for all children */
	for (/* see above */; cit != cend; cit++) {
		xmlNodePtr child = marshalToXmlNode(*cit);
		if (!xmlAddChild(xmlNode, child))
			throw XmlMarshalError("Failure creating child node");
	} 


	// add our text node if we have one
	string value = node->getXmlElementValue();
	if (value.length() != 0) {
		xmlNodePtr textNode = xmlNewText(BAD_CAST value.c_str());
		if (!xmlAddChild(xmlNode, textNode))
			throw XmlMarshalError("Failure creating text node");
	} else {
		// no value
	}

	// remove our namespace declarations from the nsDeclarationList
	for (unsigned int i = 0; i < nsDecls.size(); i++) {
		_nsDeclarationList.pop_back();
	}
	return xmlNode;
}

} // namespace
