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

#ifndef XMLMARSHALLER_H_
#define XMLMARSHALLER_H_

#include "payload.h"
#include "xmlmarshalable.h"

#include <string>

namespace ifmap2c {

class XmlMarshalError {
	private:
		std::string msg;
	public:
		XmlMarshalError(const std::string& errMsg = "") : msg(errMsg) {}

		const std::string& getMessage() const { return msg; }
};

class XmlMarshaller {

	public:
		virtual ~XmlMarshaller() { }

		/**
		 * Mashal a Marshalable object to memory.
		 *
		 * @param root the XmlMarshalable to be marshalled
		 * @return Payload object containing a pointer and
		 * 	   the length of the memory area. The caller
		 * 	   is responsible for freeing this memory.
		 */
		virtual Payload marshal(XmlMarshalable *root) = 0;
};

} // namespace
#endif /* XMLMARSHALLER_H_ */
