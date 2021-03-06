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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "xmlcommunication.h"
#include "request.h"
#include "xmlcomm/lowlevelcurlcommunication.h"
#include "typedefs.h"
#include "result.h"

using namespace std;

#define NO_IN_OUT_DEBUG

namespace ifmap2c {

XmlCommunicationError::XmlCommunicationError(const std::string& type, const std::string& msg) : _type(type), _message(msg)
{ }

XmlCommunicationError::~XmlCommunicationError()
{ }

const std::string& XmlCommunicationError::getMessage() const
{
	return _message;
}

const std::string& XmlCommunicationError::getErrorType() const
{
	return _type;
}

std::ostream& operator<<(std::ostream& output, const XmlCommunicationError& err)
{
	output << err.getErrorType() << ": ";
	output << err.getMessage();
	return output;
}

XmlCommunication::XmlCommunication(
		LowLevelCommunication *const lowLevelCom,
		XmlMarshaller *const xmlMarsh,
		XmlUnmarshaller *const xmlUnmarsh,
		RequestHandlerDispatch *const handlerDispatch) :
		_lowLevelCommunication(lowLevelCom),
		_xmlMarshaller(xmlMarsh),
		_xmlUnmarshaller(xmlUnmarsh),
		_requestHandlerDispatch(handlerDispatch)
{ }

XmlCommunication::~XmlCommunication()
{
	if (_lowLevelCommunication)
		delete _lowLevelCommunication;

	if (_xmlUnmarshaller)
		delete _xmlUnmarshaller;

	if (_xmlMarshaller)
		delete _xmlMarshaller;

	if (_requestHandlerDispatch)
		delete _requestHandlerDispatch;
}

Result *
XmlCommunication::genericRequest(Request *req)
{
	RequestHandler *handler = NULL;
	XmlMarshalable *xmlReq = NULL;
	XmlMarshalable *xmlResp = NULL;
	Result *ret = NULL;
	
	try {
		handler = _requestHandlerDispatch->dispatch(req);

		xmlReq = handler->toXml(req);

		xmlResp = xmlRequest(xmlReq);

		ret = handler->fromXml(xmlResp);

	} catch (...) {
		if (xmlReq)
			delete xmlReq;
		
		if (xmlResp)
			delete xmlResp;

		if (ret)
			delete ret;


		throw;
	}

	delete xmlReq;
	delete xmlResp;

	return ret;
}



XmlMarshalable *
XmlCommunication::xmlRequest(XmlMarshalable *const xmlMsg)
{
	Payload replyBuffer(NULL, 0), msgBuffer(NULL, 0);
	XmlMarshalable *xmlResp = NULL;

	try {
		msgBuffer = _xmlMarshaller->marshal(xmlMsg);

		replyBuffer = _lowLevelCommunication->doRequest(msgBuffer);

		xmlResp = _xmlUnmarshaller->unmarshal(replyBuffer);

	} catch (...) {
		msgBuffer.free();
		replyBuffer.free();
		throw;
	}

	msgBuffer.free();
	replyBuffer.free();
	return xmlResp;
}

} // namespace
