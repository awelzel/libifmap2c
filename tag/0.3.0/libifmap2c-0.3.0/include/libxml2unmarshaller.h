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

#ifndef LIBXML2UNMARSHALLER_H_
#define LIBXML2UNMARSHALLER_H_

#include "xmlmarshalable.h"
#include "basicxmlmarshalable.h"
#include "payload.h"
#include "xmlunmarshaller.h"

extern "C" {
	#include <libxml/parser.h>
	#include <libxml/tree.h>
}

namespace ifmap2c {

class LibXml2Unmarshaller : public XmlUnmarshaller {

public:
	XmlMarshalable *unmarshal(const Payload& p);

	static XmlUnmarshaller *
	createUnmarshaller();

private:
	LibXml2Unmarshaller() { };

	XmlMarshalable *
	unmarshalXmlNode(xmlNodePtr const node);
};

} // namespace
#endif /* LIBXML2UNMARSHALLER_H_ */
