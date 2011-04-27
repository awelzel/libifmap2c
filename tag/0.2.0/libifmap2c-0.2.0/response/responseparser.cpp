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

#include "responseparser.h"
#include "typedefs.h"
#include "tcgifmapbase.h"

#include <cstring>
#include <sstream>
#include <iostream>

using namespace std;

namespace ifmap2c {


//TODO: If we gave a max poll result size in the request, we have
//      to get one back!
NewSessionResult *
ResponseParser::createNewSessionResult(XmlMarshalable *const env)
{
	string pId, sId, maxPs;
	bool pIdFound = false, sIdFound = false;
	XmlMarshalable *newSessionResult = locateNewSessionResultElement(env);
	throwIfNull(newSessionResult, NEWSESSIONRESULT_ELEMENT_NAME);

	CSTRPLISTIT it = newSessionResult->getXmlAttributes().begin();
	CSTRPLISTIT end = newSessionResult->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (isAttrWithName(*it, PUBLISHERID_ATTR_NAME)) {
			pId = it->second;
			pIdFound = true;
		} else if (isAttrWithName(*it, SESSIONID_ATTR_NAME)) {
			sId = it->second;
			sIdFound = true;
		} else if (isAttrWithName(*it, MAX_POLL_RES_SIZE_ATTR_NAME)) {
			maxPs = it->second;
		} else {
			// unknown attribute
		}
	}

	if (!pIdFound)
		throw ResponseParseError("PublisherId not found");

	if(!sIdFound)
		throw ResponseParseError("SessionId not found");

	return new NewSessionResult(sId, pId, maxPs);
}


void
ResponseParser::checkPublishReceived(XmlMarshalable *const env)
{
	XmlMarshalable *publishRecv = locatePublishReceivedElement(env);
	throwIfNull(publishRecv, PUBLISHRECV_ELEMENT_NAME);
}


void
ResponseParser::checkSubscribeReceived(XmlMarshalable *const env)
{
	XmlMarshalable *subscribeRecv = locateSubscribeReceivedElement(env);
	throwIfNull(subscribeRecv, SUBSCRIBERECV_ELEMENT_NAME);
}



void
ResponseParser::checkEndSessionResult(XmlMarshalable *const env)
{
	XmlMarshalable *endsessionRes = locateEndSessionResultElement(env);
	throwIfNull(endsessionRes, ENDSESSIONRESULT_ELEMENT_NAME);
}


void
ResponseParser::checkRenewSessionResult(XmlMarshalable *const env)
{
	XmlMarshalable *renewSessionRes = locateRenewSessionResultElement(env);
	throwIfNull(renewSessionRes, RENEWSESSIONRESULT_ELEMENT_NAME);
}

void
ResponseParser::checkPurgePublisherReceived(XmlMarshalable *const env)
{
	XmlMarshalable *purgePublisherRecv = locatePurgePublisherRecvElement(env);
	throwIfNull(purgePublisherRecv, PURGEPUBLISHERRECV_ELEMENT_NAME);
}


SearchResult *
ResponseParser::createSearchResult(XmlMarshalable *const env)
{
	XmlMarshalable *xmlSearchRes = NULL;

	xmlSearchRes = locateSearchResultElement(env);
	throwIfNull(xmlSearchRes, SEARCH_ELEMENT_NAME);

	return extractSearchResult(xmlSearchRes);
}

SearchResult *
ResponseParser::extractSearchResult(XmlMarshalable *const sr)
{
	SearchResult *retSearchRes = NULL;
	XmlMarshalable *child = NULL;
	std::string searchResultName;

	searchResultName = extractSearchResultName(sr);

	CXMLMLISTIT it = sr->getXmlChildren().begin();
	CXMLMLISTIT end = sr->getXmlChildren().end();

	retSearchRes = new SearchResult(searchResultName);

	// we have result items down here
	try {
		for (/* see above */; it != end; it++) {
			child = *it;
			retSearchRes->addResultItem(createResultItem(child));
		}
	} catch (...) {
		// cleanup
		delete retSearchRes;
		throw;
	}

	return retSearchRes;

}

PollResult *
ResponseParser::createPollResult(XmlMarshalable *const env)
{
	PollResult *retPollRes = NULL;
	XmlMarshalable *xmlPollRes = NULL;
	XmlMarshalable *child = NULL;
	SearchResult *sr = NULL;
	xmlPollRes = locatePollResultElement(env);

	if (xmlPollRes == NULL) {
		xmlPollRes = locateEndSessionResultElement(env);
		if (xmlPollRes) // we received a endSessionResult
			throw EndSessionResult();
	}

	throwIfNull(xmlPollRes, POLLRESULT_ELEMENT_NAME);

	retPollRes = new PollResult();

	// we have SearchResult elements down here
	CXMLMLISTIT it = xmlPollRes->getXmlChildren().begin();
	CXMLMLISTIT end = xmlPollRes->getXmlChildren().end();

	try {
		for (/* see above */; it != end; ++it) {
			child = *it;
			if (compNameNs(child, POLLRESULT_SEARCH_ELEMENT_NAME,
					POLLRESULT_SEARCH_ELEMENT_HREF)) {
				sr = extractSearchResult(child);
				retPollRes->addSearchResult(sr);
			} else if (compNameNs(child, POLLRESULT_UPDATE_ELEMENT_NAME,
					POLLRESULT_UPDATE_ELEMENT_HREF)) {
				sr = extractSearchResult(child);
				retPollRes->addUpdateResult(sr);
			} else if (compNameNs(child, POLLRESULT_DELETE_ELEMENT_NAME,
					POLLRESULT_DELETE_ELEMENT_HREF)) {
				sr = extractSearchResult(child);
				retPollRes->addDeleteResult(sr);
			} else if (compNameNs(child, POLLRESULT_NOTIFY_ELEMENT_NAME,
					POLLRESULT_NOTIFY_ELEMENT_HREF)) {
				sr = extractSearchResult(child);
				retPollRes->addNotifyResult(sr);
			} else if (compNameNs(child, POLLRESULT_ERROR_ELEMENT_NAME,
					POLLRESULT_ERROR_ELEMENT_HREF)) {
				// TODO: could throw a PollErrorResult here?
				throwErrorResult(child);
			} else {
				throw ResponseParseError("Bad element in PollResult");
			}
		}
	} catch (...) {
		if (retPollRes)
			delete retPollRes;
		throw;
	}
	return retPollRes;
}


std::string
ResponseParser::extractSearchResultName(XmlMarshalable *const sr)
{
	CSTRPLISTIT it = sr->getXmlAttributes().begin();
	CSTRPLISTIT end = sr->getXmlAttributes().end();

	for (/* */; it != end; it++) {
		if (isAttrWithName(*it, SEARCHRESULT_NAME_ATTR_NAME)) {
			return (*it).second;
		}
	}
	return "";
}



ResultItem *
ResponseParser::createResultItem(XmlMarshalable *xmlResultItem)
{
	AccessRequest *ar = NULL;
	IpAddress *ip = NULL;
	MacAddress *mac = NULL;
	Device *dev = NULL;
	Identity *id = NULL;
	XmlMarshalable *child = NULL;
	XmlMarshalable *mdList = NULL;

	ResultItem *resultItem = NULL;

	CXMLMLIST childs = xmlResultItem->getXmlChildren();

	int nChildren = childs.size();

	if (nChildren > 3 || nChildren < 1)
		throw ResponseParseError("ResultItem contains wrong number of elements");

	CXMLMLISTIT it = childs.begin();
	CXMLMLISTIT end = childs.end();

	try {
		resultItem = new ResultItem();
		for (/* see above */; it != end; it++) {
			child = *it;
			if ((ar = extractAccessRequest(child))) {
				resultItem->setAccessRequest(ar);
				ar = NULL;
			} else if ((ip = extractIpAddress(child))) {
				resultItem->setIpAddress(ip);
				ip = NULL;
			} else if ((mac = extractMacAddress(child))) {
				resultItem->setMacAddress(mac);
				mac = NULL;
			} else if ((dev = extractDevice(child))) {
				resultItem->setDevice(dev);
				dev = NULL;
			} else if ((id = extractIdentity(child))) {
				resultItem->setIdentity(id);
				id = NULL;
			} else if ((mdList = extractMetadataList(child))) {
				CXMLMLISTIT it = mdList->getXmlChildren().begin();
				CXMLMLISTIT end = mdList->getXmlChildren().end();

				// hang children into the searchResult
				for (/* see above */; it != end; it++)
					resultItem->addMetadata(*it);

				// remove them from the parsed XML document
				mdList->clearXmlChildren();
			}
		}
	} catch (ResponseParseError e) {
		delete resultItem;

		if (!ar)
			delete ar;

		if (!ip)
			delete ip;

		if (!mac)
			delete mac;

		if (!dev)
			delete dev;

		if (!id)
			delete id;

		throw ResponseParseError(e.getMessage());
		delete resultItem;
		throw;
	}

	return resultItem;
}



AccessRequest *
ResponseParser::extractAccessRequest(XmlMarshalable *const element)
{
	AccessRequest *ar = NULL;
	bool foundName = false;
	string name, ad;

	if (compNameNs(element, ACCESSREQ_ELEMENT_NAME, ACCESSREQ_ELEMENT_HREF)) {
		CSTRPLISTIT it = element->getXmlAttributes().begin();
		CSTRPLISTIT end = element->getXmlAttributes().end();

		for (/* see above */; it != end; it++) {
			if (isAttrWithName(*it, ACCESSREQ_NAME_ATTR_NAME)) {
				name = (*it).second;
				foundName = true;
			} else if (isAttrWithName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
				ad = (*it).second;
			}
		}

		if (!foundName)
			throw ResponseParseError("Got AccessRequest without name");

		ar = AccessRequest::createAccessRequest(name, ad);

	}
	return ar;
}



IpAddress *
ResponseParser::extractIpAddress(XmlMarshalable *const element)
{
	IpAddress *ip = NULL;
	IpAddressType type = ipv4;
	bool foundValue = false;
	string value, ad;

	if (compNameNs(element, IPADDR_ELEMENT_NAME, IPADDR_ELEMENT_HREF)) {
		CSTRPLISTIT it = element->getXmlAttributes().begin();
		CSTRPLISTIT end = element->getXmlAttributes().end();

		for (/* see above */; it != end; it++) {
			if (isAttrWithName(*it, IPADDR_VALUE_ATTR_NAME)) {
				value = (*it).second;
				foundValue = true;
			} else if (isAttrWithName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
				ad = (*it).second;
			} else if (isAttrWithName(*it, IPADDR_TYPE_ATTR_NAME)) {
				if (!(*it).second.compare(IPADDR_TYPE_IPV6)) {
					type = ipv6;
				}
			}
		}

		if (!foundValue)
			throw ResponseParseError("Got IpAddress without value");

		if (type == ipv4) {
			ip = IpAddress::createIpv4Address(value, ad);
		} else {
			ip = IpAddress::createIpv6Address(value, ad);
		}
	}

	return ip;
}

MacAddress *
ResponseParser::extractMacAddress(XmlMarshalable *const element)
{
	MacAddress *mac = NULL;
	bool valueFound = false;
	string value, ad;

	if (compNameNs(element, MACADDR_ELEMENT_NAME, MACADDR_ELEMENT_HREF)) {
		CSTRPLISTIT it = element->getXmlAttributes().begin();
		CSTRPLISTIT end = element->getXmlAttributes().end();

		for (/* see above */; it != end; it++) {
			if (isAttrWithName(*it, MACADDR_VALUE_ATTR_NAME)) {
				value = (*it).second;
				valueFound = true;
			} else if (isAttrWithName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
				ad = (*it).second;
			}
		}

		if (!valueFound)
			throw ResponseParseError("Got MacAddress without value");

		mac = MacAddress::createMacAddress(value, ad);
	}

	return mac;
}



Device *
ResponseParser::extractDevice(XmlMarshalable *const element)
{
	Device *dev = NULL;
	string elementValue;

	if (compNameNs(element, DEVICE_ELEMENT_NAME, DEVICE_ELEMENT_HREF)) {

		if (element->getXmlChildren().size() > 1)
			throw ResponseParseError("Device with more than"
					"one child element");

		CXMLMLISTIT it = element->getXmlChildren().begin();

		if (compNameNs(*it, DEVICE_NAME_ELEMENT, DEVICE_NAME_HREF)) {
			elementValue = (*it)->getXmlElementValue();
		} else {
			throw ResponseParseError("Device does not contain "
					"a name element");
		}
		dev = Device::createDevice(elementValue);

	}
	return dev;
}



Identity *
ResponseParser::extractIdentity(XmlMarshalable *const element)
{
	Identity *id = NULL;
	bool foundType = false, foundName = false, foundOther = false;
	string typeStr, name, otherDefStr, ad;
	IdentityType type;

	if (compNameNs(element, IDENTITY_ELEMENT_NAME, IDENTITY_ELEMENT_HREF)) {

		CSTRPLISTIT it = element->getXmlAttributes().begin();
		CSTRPLISTIT end = element->getXmlAttributes().end();

		for (/* see above */; it != end; it++) {
			if (isAttrWithName(*it, IDENTITY_TYPE_ATTR_NAME)) {
				typeStr = (*it).second;
				foundType = true;
			} else if (isAttrWithName(*it, IDENTITY_NAME_ATTR_NAME)) {
				name = (*it).second;
				foundName = true;
			} else if (isAttrWithName(*it, IDENTITY_OTHER_DEF_ATTR_NAME)) {
				otherDefStr = (*it).second;
				foundOther = true;
			} else if (isAttrWithName(*it, ADMIN_DOMAIN_ATTR_NAME)) {
				ad = (*it).second;
			}
		}
		
		if (!foundType)
			throw ResponseParseError("Got Identity without type");

		if (!foundName)
			throw ResponseParseError("Got Identity without name");

		
		if (!typeStr.compare(Identity::identityTypeNames[aik_name])) {
			type = aik_name;
		} else if (!typeStr.compare(Identity::identityTypeNames[distinguished_name])) {
			type = distinguished_name;
		} else if (!typeStr.compare(Identity::identityTypeNames[dns_name])) {
			type = dns_name;
		} else if (!typeStr.compare(Identity::identityTypeNames[email_address])) {
			type = email_address;
		} else if (!typeStr.compare(Identity::identityTypeNames[hip_hit])) {
			type = hip_hit;
		} else if (!typeStr.compare(Identity::identityTypeNames[kerberos_principal])) {
			type = kerberos_principal;
		} else if (!typeStr.compare(Identity::identityTypeNames[username])) {
			type = username;
		} else if (!typeStr.compare(Identity::identityTypeNames[sip_uri])) {
			type = sip_uri;
		} else if (!typeStr.compare(Identity::identityTypeNames[tel_uri])) {
			type = tel_uri;
		} else if (!typeStr.compare(Identity::identityTypeNames[other])) {
			type = other;
		} else {
			throw ResponseParseError("Got Identity with unknown type");
		}

		if (type == other && !foundOther)
			throw ResponseParseError("Identity with other type definition"
					" found, but no other-type-definition attribute");

		if (type == other)
			id = Identity::createOtherIdentity(otherDefStr, name, ad);
		else
			id = Identity::createIdentity(type, name, ad);
	}

	return id;
}



XmlMarshalable *
ResponseParser::extractMetadataList(XmlMarshalable *const element)
{
	if (compNameNs(element, METADATA_LIST_ELEMENT_NAME, METADATA_LIST_ELEMENT_HREF))
		return element;

	return NULL;
}



XmlMarshalable *
ResponseParser::locateResponseElement(XmlMarshalable *const env)
{
	XmlMarshalable *body = NULL, *response = NULL;
	body = getChild(env, BODY_ELEMENT_NAME, BODY_ELEMENT_HREF);
	throwIfNull(body, BODY_ELEMENT_NAME);
	response = getChild(body, RESPONSE_ELEMENT_NAME, RESPONSE_ELEMENT_HREF);
	return response;
}



XmlMarshalable *
ResponseParser::locateUnderResponse(XmlMarshalable *const env, const STRP& eldesc)
{
	XmlMarshalable *element = NULL, *response = NULL;
	response = locateResponseElement(env);
	throwIfNull(response, RESPONSE_ELEMENT_NAME);
	checkErrorResult(response);
	element = getChild(response, eldesc.first, eldesc.second);
	return element;
}



XmlMarshalable *
ResponseParser::locateNewSessionResultElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(NEWSESSIONRESULT_ELEMENT_NAME,
			NEWSESSIONRESULT_ELEMENT_HREF));
}



XmlMarshalable *
ResponseParser::locateSearchResultElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(SEARCHRESULT_ELEMENT_NAME,
			SEARCHRESULT_ELEMENT_HREF));
}

XmlMarshalable *
ResponseParser::locatePollResultElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(POLLRESULT_ELEMENT_NAME,
			POLLRESULT_ELEMENT_HREF));
}



XmlMarshalable *
ResponseParser::locateEndSessionResultElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(ENDSESSIONRESULT_ELEMENT_NAME,
			ENDSESSIONRESULT_ELEMENT_HREF));
}



XmlMarshalable *
ResponseParser::locateSubscribeReceivedElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(SUBSCRIBERECV_ELEMENT_NAME,
			SUBSCRIBERECV_ELEMENT_HREF));
}
XmlMarshalable *
ResponseParser::locateRenewSessionResultElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(RENEWSESSIONRESULT_ELEMENT_NAME,
			RENEWSESSIONRESULT_ELEMENT_HREF));
}

XmlMarshalable *
ResponseParser::locatePurgePublisherRecvElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(PURGEPUBLISHERRECV_ELEMENT_NAME,
			PURGEPUBLISHERRECV_ELEMENT_HREF));
}



XmlMarshalable *
ResponseParser::locatePublishReceivedElement(XmlMarshalable *const env)
{
	return locateUnderResponse(env, STRP(PUBLISHRECV_ELEMENT_NAME,
			PUBLISHRECV_ELEMENT_HREF));
}



bool
ResponseParser::compNameNs(XmlMarshalable *const el,
		const std::string& name,
		const std::string& href)
{
	return XmlMarshalable::compNameNs(el, name, href);
}



bool
ResponseParser::isAttrWithName(const STRP& attr, const string& attrname)
{
	return !attr.first.compare(attrname);
}



XmlMarshalable *
ResponseParser::getChild(XmlMarshalable *marsh, const string& elname, const string& href)
{
	XMLMLIST children = marsh->getXmlChildren();
	XMLMLISTIT it = children.begin();
	XMLMLISTIT end = children.end();

	for (/* see above */; it != end; it++)
		if(compNameNs(*it, elname, href))
			return *it;

	return NULL;
}



void
ResponseParser::checkErrorResult(XmlMarshalable *const response)
{
	XmlMarshalable *errorResult = getChild(response, ERRORRESULT_ELEMENT_NAME,
			ERRORRESULT_ELEMENT_HREF);

	if (errorResult != NULL)
		throwErrorResult(errorResult);
}



void
ResponseParser::throwErrorResult(XmlMarshalable *const err)
{
	ErrorCode errCode = Unknown;
	string errCodeString;
	string errString;

	CSTRPLISTIT it = err->getXmlAttributes().begin();
	CSTRPLISTIT end = err->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (!it->first.compare(ERRORCODE_ATTR_NAME)) {
			errCodeString = it->second;
		}
	}

	XmlMarshalable *errStrEle = getChild(err, ERRORSTRING_ELEMENT_NAME,
			ERRORSTRING_ELEMENT_HREF);

	if (errStrEle != NULL) {
		errString = errStrEle->getXmlElementValue();
	}



	// Oh god... FIXME
	if (!errCodeString.compare(ErrorResultError::errorCodeStrings[AccessDenied])) {
		errCode = AccessDenied;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[Failure])) {
		errCode = Failure;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[InvalidIdentifier])) {
		errCode = InvalidIdentifier;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[InvalidIdentifierType])) {
		errCode = InvalidIdentifierType;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[IdentifierTooLong])) {
		errCode = IdentifierTooLong;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[InvalidMetadata])) {
		errCode = InvalidMetadata;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[InvalidSchemaVersion])) {
		errCode = InvalidSchemaVersion;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[InvalidSessionId])) {
		errCode = InvalidSessionId;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[MetadataTooLong])) {
		errCode = MetadataTooLong;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[SearchResultsTooBig])) {
		errCode = SearchResultsTooBig;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[PollResultsTooBig])) {
		errCode = PollResultsTooBig;
	} else if (!errCodeString.compare(ErrorResultError::errorCodeStrings[SystemError])) {
		errCode = SystemError;;
	} else {
		errCode = Unknown;
	}

	throw ErrorResultError(errCode, errString);
}



void
ResponseParser::throwIfNull(XmlMarshalable *const m, const string& msg)
{
	if (m == NULL) {
		stringstream ss;
		ss << "No ";
		ss << msg;
		ss << " element found.";
		throw ResponseParseError(ss.str());
	}
}

} // namespace
