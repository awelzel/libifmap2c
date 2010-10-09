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

#ifndef XMLMARSHALABLE_H_
#define XMLMARSHALABLE_H_

#include "tcgifmapbase.h"

#include <string>
#include <list>

namespace LIBRARY_NAMESPACE {

class XmlMarshalable {
public:
	virtual ~XmlMarshalable() { }

	virtual const std::string& getXmlElementName(void) const = 0;

	virtual void
	addXmlAttribute(const std::pair<std::string, std::string>& ns) = 0;

	virtual const std::list<std::pair<std::string, std::string> >&
	getXmlAttributes(void) const = 0;

	virtual const std::list<XmlMarshalable *>& getXmlChildren(void) const = 0;

	virtual const std::string& getXmlElementValue(void) const = 0;

	virtual void
	addXmlNamespaceDefinition(const std::pair<std::string, std::string>& attr) = 0;

	virtual const std::list<std::pair<std::string, std::string> >&
	getXmlNamespaceDefinitions(void) const = 0;
		
	virtual const std::pair<std::string, std::string>&
	getXmlNamespace() const = 0;

	virtual void addXmlChild(XmlMarshalable *const child) = 0;

	virtual void clearXmlChildren(void) = 0;

	static void putXmlMarshalable(XmlMarshalable *m);
};

} // namespace
#endif /* XMLMARSHALABLE_H_ */
