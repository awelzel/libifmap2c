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

#ifndef IFMAPCOMMUNICATION_H_
#define IFMAPCOMMUNICATION_H_
#include <string>

//FIXME: Get rid of ifmap dependencies if possible
#include "communicationerror.h"
#include "ifmaperror.h"
#include "lowlevelcommunication.h"
//#include "responseparseerror.h"
//#include "responseparser.h"
#include "request.h"
#include "result.h"
#include "xmlbinding.h"

namespace ifmap2c {


class XmlCommunication {

public:
	virtual ~XmlCommunication();

	Result *genericRequest(Request *const req);

	XmlCommunication(LowLevelCommunication *const lowLevelCom,
			XmlMarshaller *const xmlMarsh,
			XmlUnmarshaller *const xmlUnmarsh,
			RequestHandlerDispatch *const handlerDispatch);

private:
	LowLevelCommunication *const _lowLevelCommunication;
	XmlMarshaller *const _xmlMarshaller;
	XmlUnmarshaller *const _xmlUnmarshaller;
	RequestHandlerDispatch *const _requestHandlerDispatch;

	/**
	 *
	 */
	XmlMarshalable *xmlRequest(XmlMarshalable *const xmlMsg);
};

} // namespace

#endif /* IFMAPCOMMUNICATION_H_ */
