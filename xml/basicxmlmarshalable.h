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

#ifndef BASICXMLMARSHALABLE_H_
#define BASICXMLMARSHALABLE_H_

#include "xmlmarshalable.h"
#include <list>
#include <string>

namespace LIBRARY_NAMESPACE {

class BasicXmlMarshalable : public XmlMarshalable {

public:
	BasicXmlMarshalable(const std::string& name,
		const std::string& val,
		const std::pair<std::string, std::string>& ns);

	virtual ~BasicXmlMarshalable();

	const std::string&
	getXmlElementName(void) const;

	const std::string&
	getXmlElementValue(void) const;

	const std::pair<std::string, std::string>&
	getXmlNamespace(void) const;

	const std::list<std::pair<std::string, std::string> >&
	getXmlNamespaceDefinitions(void) const;

	const std::list<std::pair<std::string, std::string> >&
	getXmlAttributes(void) const;

	const std::list<XmlMarshalable *>& getXmlChildren(void) const;
		
	void addXmlChild(XmlMarshalable *const child);

	void clearXmlChildren(void);

	void addXmlAttribute(const std::pair<std::string, std::string>& attr);

	void addXmlNamespaceDefinition(const std::pair<std::string, std::string>& ns);
private:
	std::string _xmlElementName;
	std::string _xmlElementValue;
	std::pair<std::string, std::string> _xmlNamespace;
	std::list<std::pair<std::string, std::string> > _xmlNamespaceDeclarations;
	std::list<std::pair<std::string, std::string> > _xmlAttributes;
	std::list<XmlMarshalable *> _xmlChildren;
};

} // namespace
#endif // BASICXMLMARSHALABLE_H_
