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

#include "ifmapcommunication.h"
#include "request.h"
#include "xmlmarshalable.h"
#include "xml/libxml2marshaller.h"
#include "xml/libxml2unmarshaller.h"
#include "communication/lowlevelcurlcommunication.h"
#include "typedefs.h"
#include "result.h"

#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

#define NO_IN_OUT_DEBUG

namespace ifmap2c {

IfmapCommunication::IfmapCommunication(const string& url,
		const string& user,
		const string& pass,
		const string& capath) :
		_basicAuth(true),
		_url(url),
		_userName(user),
		_password(pass),
		_caPath(capath)
{
	_lowLevelCommunication = LowLevelCurlCommunication::create(url, user, pass, capath);
	_xmlMarshaller = LibXml2Marshaller::createMarshaller();
	_xmlUnmarshaller = LibXml2Unmarshaller::createUnmarshaller();
	_requestHandlerDispatch = 
		RequestHandlerDispatch::createRequestHandlerDispatch();
}

IfmapCommunication::IfmapCommunication(const string& url,
		const string& mykey,
		const string& mykeypw,
		const string& mycert,
		const string& capath) :
		_basicAuth(false),
		_url(url),
		_keyFile(mykey),
		_certFile(mycert),
		_password(mykeypw),
		_caPath(capath)
{
	_lowLevelCommunication = LowLevelCurlCommunication::create(url,
			mykey, mykeypw, mycert, capath);
	_xmlMarshaller = LibXml2Marshaller::createMarshaller();
	_xmlUnmarshaller = LibXml2Unmarshaller::createUnmarshaller();
}


IfmapCommunication::~IfmapCommunication()
{
	if (_lowLevelCommunication)
		delete _lowLevelCommunication;

	if (_xmlUnmarshaller)
		delete _xmlUnmarshaller;

	if (_xmlMarshaller)
		delete _xmlMarshaller;

	if (_requestHandlerDispatch)
		delete _requestHandlerDispatch;

	_lowLevelCommunication = NULL;
	_xmlUnmarshaller = NULL;
	_xmlMarshaller = NULL;
	_requestHandlerDispatch = NULL;
}


bool
IfmapCommunication::containsSessionId(XmlMarshalable *req)
{
	// if this is a newSession, then return true in any case...
	if (ResponseParser::compNameNs(req, NEWSESSION_ELEMENT_NAME,
				NEWSESSION_ELEMENT_HREF)) {
		return true;
	}

	CSTRPLISTIT it = req->getXmlAttributes().begin();
	CSTRPLISTIT end = req->getXmlAttributes().end();

	for (/* */; it != end; it++) {
		if (ResponseParser::isAttrWithName(*it,
					SESSIONID_ATTR_NAME)) {
			return true;
		}
	}

	return false;
}


XmlMarshalable *
IfmapCommunication::buildEnvelope()
{

	XmlMarshalable *envelope = new BasicXmlMarshalable(
			ENVELOPE_ELEMENT_NAME,
			EMPTY_VALUE,
			SOAP_NSPAIR);

	envelope->addXmlNamespaceDefinition(STRP(SOAP_PREFIX, SOAP_HREF));
	envelope->addXmlNamespaceDefinition(STRP(IFMAP_PREFIX, IFMAP_HREF));

	XmlMarshalable *body = new BasicXmlMarshalable(
			BODY_ELEMENT_NAME,
			EMPTY_VALUE,
			SOAP_NSPAIR);

	envelope->addXmlChild(body);

	return envelope;
}

Result *
IfmapCommunication::genericRequest(Request *req, const string& sId)
{
	RequestHandler *handler = NULL;
	XmlMarshalable *xmlContent = NULL;
	XmlMarshalable *xmlReq = NULL;
	XmlMarshalable *xmlResp = NULL;
	Result *ret = NULL;

	try {
		handler = _requestHandlerDispatch->dispatch(req);
		xmlContent = handler->toXml(req);
		
		if (sId.length() > 0)
			setSessionId(xmlContent, sId);

		xmlReq = buildEnvelope();
		(*(xmlReq->getXmlChildren().begin()))->addXmlChild(xmlContent);
		xmlContent = NULL;

		xmlResp = xmlRequest(xmlReq);

		ret = handler->fromXml(xmlResp);

	} catch (...) {

		if (xmlReq)
			delete xmlReq;
		
		if (xmlContent)
			delete xmlContent;

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
IfmapCommunication::xmlRequest(XmlMarshalable *const xmlMsg)
{
	Payload replyBuffer(NULL, 0), msgBuffer(NULL, 0);
	XmlMarshalable *xmlResp = NULL;

#ifdef IN_OUT_DEBUG
	cout << "======== OUTGOING =========" << endl;
	XmlMarshalable::putXmlMarshalable(env);
	cout << "======== OUTGOING =========" << endl;
#endif /* IN_OUT_DEBUG */

	try {
		msgBuffer = _xmlMarshaller->marshal(xmlMsg);
		replyBuffer = _lowLevelCommunication->doRequest(msgBuffer);
		xmlResp = _xmlUnmarshaller->unmarshal(replyBuffer);
	} catch (...) {
		msgBuffer.free();
		replyBuffer.free();
		throw;
	}

#ifdef IN_OUT_DEBUG
	cout << "======== INCOMING =========" << endl;
	XmlMarshalable::putXmlMarshalable(xmlResp);
	cout << "======== INCOMING =========" << endl;
#endif /* IN_OUT_DEBUG */

	msgBuffer.free();
	replyBuffer.free();
	return xmlResp;
}

void
IfmapCommunication::setSessionId(XmlMarshalable *const req,
		const string& sessionId)
{
	CSTRPLISTIT it = req->getXmlAttributes().begin();
	CSTRPLISTIT end = req->getXmlAttributes().end();
	STRP strp;
	// if we find a session-id attribute we need to copy the whole list :-(

	for (/* see above */; it != end; it++) {
		strp = *it;
		if (!strp.first.compare(SESSIONID_ATTR_NAME)) {
			STRPLIST attrList = req->getXmlAttributes();
			req->clearXmlAttributes();

			it = attrList.begin();
			end = attrList.end();

			for (STRP strp = *it; it != end; *(++it)) {
				if (strp.first.compare(SESSIONID_ATTR_NAME)) {
					req->addXmlAttribute(strp);
				}
			}
			break;
		}
	}
	req->addXmlAttribute(STRP(SESSIONID_ATTR_NAME, sessionId));
}

} // namespace
