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

#include "ifmap/baserequesthandler.h"
#include "identifier.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"
#include <string>


using namespace std;

namespace ifmap2c {

//static IdentifierHandlerDispatch identDispatch;

/*	
XmlMarshalable *
XmlCommunication::buildEnvelope()
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
*/

	/*	TODO: To this in the appropiate handlers
		if (sId.length() > 0)
			setSessionId(xmlContent, sId);

		xmlReq = buildEnvelope();
		(*(xmlReq->getXmlChildren().begin()))->addXmlChild(xmlContent);
		xmlContent = NULL;


	*/

/*
bool
XmlCommunication::containsSessionId(XmlMarshalable *req)
{
	// if this is a newSession, then return true in any case...
	if (ResponseParser::compNameNs(req, NEWSESSION_ELEMENT_NAME,
				NEWSESSION_ELEMENT_HREF)) {
		return true;
	}

	CSTRPLISTIT it = req->getXmlAttributes().begin();
	CSTRPLISTIT end = req->getXmlAttributes().end();

	for (/ /; it != end; it++) {
		if (ResponseParser::isAttrWithName(*it,
					SESSIONID_ATTR_NAME)) {
			return true;
		}
	}

	return false;
}
*/

IFMAP2C_RH_TOXML_DEF(NewSessionRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(NewSessionRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(EndSessionRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(EndSessionRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(RenewSessionRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(RenewSessionRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(PurgePublisherRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(PurgePublisherRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(PublishRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(PublishRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(SearchRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(SearchRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(SubscribeRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(SubscribeRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(PollRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(PollRequest, xml)
{
	return (Result*)xml;
}

}
