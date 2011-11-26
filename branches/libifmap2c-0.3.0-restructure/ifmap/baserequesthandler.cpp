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
#include "requests.h"
#include "results.h"
#include "identifier.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"
#include "errorresult.h"

#include <iostream>
#include <string>
#include <sstream>


using namespace std;

namespace ifmap2c {

static IdentifierHandlerDispatch identDispatch;

static void
throwIfNull(XmlMarshalable *const m, const string& msg)
{
	if (m == NULL) {
		stringstream ss;
		ss << "No ";
		ss << msg;
		ss << " element found.";
		throw RequestHandlerError(ss.str());
	}
}

static bool
cmpName(XmlMarshalable *const el, const string& name, const string& href)
{
	return XmlMarshalable::compNameNs(el, name, href);
}

	static bool
cmpName(XmlMarshalable *const el, const string& name)
{
	return XmlMarshalable::compNameNs(el, name, NO_HREF);
}

static bool
cmpAttrName(const STRP& attr, const string& attrname)
{
	return !attr.first.compare(attrname);
}

static XmlMarshalable *
findChild(XmlMarshalable *marsh, const STRP& el)
{
	XMLMLIST children = marsh->getXmlChildren();
	XMLMLISTIT it = children.begin();
	XMLMLISTIT end = children.end();

	for (/* see above */; it != end; it++)
		if(cmpName(*it, el.first, el.second))
			return *it;

	return NULL;
}

static XmlMarshalable *
findChildChecked(XmlMarshalable *marsh, const STRP& el)
{
	XmlMarshalable *ret = findChild(marsh, el);
	throwIfNull(ret, el.first);
	
	return ret;
}

// FIX THIS!!, USE A HASH MAP!!!
static ErrorResult *
createErrorResult(XmlMarshalable *const err)
{
	ErrorCode errCode = Unknown;
	XmlMarshalable *errStrEle = NULL;
	string errCodeString;
	string errString;
	string errName;

	CSTRPLISTIT it = err->getXmlAttributes().begin();
	CSTRPLISTIT end = err->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (!it->first.compare(ERRORCODE_ATTR_NAME))
			errCodeString = it->second;
		else if (!it->first.compare(ERRORSUBSCRIPTON_ATTR_NAME))
			errName = it->second;
	}

	errStrEle = findChild(err, STRP(ERRORSTRING_ELEMENT_NAME,
			ERRORSTRING_ELEMENT_HREF));

	if (errStrEle)
		errString = errStrEle->getXmlElementValue();

	// Oh god... FIXME
	if (!errCodeString.compare(ErrorResult::errorCodeStrings[AccessDenied])) {
		errCode = AccessDenied;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[Failure])) {
		errCode = Failure;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[InvalidIdentifier])) {
		errCode = InvalidIdentifier;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[InvalidIdentifierType])) {
		errCode = InvalidIdentifierType;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[IdentifierTooLong])) {
		errCode = IdentifierTooLong;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[InvalidMetadata])) {
		errCode = InvalidMetadata;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[InvalidSchemaVersion])) {
		errCode = InvalidSchemaVersion;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[InvalidSessionID])) {
		errCode = InvalidSessionID;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[MetadataTooLong])) {
		errCode = MetadataTooLong;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[SearchResultsTooBig])) {
		errCode = SearchResultsTooBig;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[PollResultsTooBig])) {
		errCode = PollResultsTooBig;
	} else if (!errCodeString.compare(ErrorResult::errorCodeStrings[SystemError])) {
		errCode = SystemError;
	}

	return new ErrorResult(errCode, errString, errName);
}

static void
addXmlNamespaceDefs(XmlMarshalable *const to, IfmapRequest *const from)
{
	list<pair<string, string> >::const_iterator it, end;
	it = from->getXmlNamespaceDefinitions().begin();
	end = from->getXmlNamespaceDefinitions().end();

	for (/* */; it != end; it++)
		to->addXmlNamespaceDefinition(*it);

}

static void
throwIfError(XmlMarshalable *const m)
{
	ErrorResult *error = NULL;
	ErrorResult toThrow;
	XmlMarshalable *errorResult = findChild(m,
			STRP(ERRORRESULT_ELEMENT_NAME,
			ERRORRESULT_ELEMENT_HREF));

	if (errorResult != NULL) {
		error = createErrorResult(errorResult);
		toThrow = *error;
		delete error;
		throw toThrow;
	}
}
XmlMarshalable *
findIfmapResponseChecked(XmlMarshalable *const xml)
{
	XmlMarshalable *body = 
		findChildChecked(xml, STRP(BODY_ELEMENT_NAME,
				BODY_ELEMENT_HREF));

	return findChildChecked(body,
			STRP(RESPONSE_ELEMENT_NAME,
			RESPONSE_ELEMENT_HREF));
}

XmlMarshalable *
findIfmapResultChecked(const string& el, XmlMarshalable *const xml)
{
	XmlMarshalable *response;
	
	response = findIfmapResponseChecked(xml);
	
	throwIfError(response);
	
	return findChildChecked(response, STRP(el, NO_HREF));
}

static XmlMarshalable *
getFor(const string& elname, XmlMarshalable **root)
{
	*root = new BasicXmlMarshalable(
			ENVELOPE_ELEMENT_NAME,
			EMPTY_VALUE,
			SOAP_NSPAIR);

	(*root)->addXmlNamespaceDefinition(
			STRP(SOAP_PREFIX, SOAP_HREF));

	(*root)->addXmlNamespaceDefinition(
			STRP(IFMAP_PREFIX, IFMAP_HREF));

	XmlMarshalable *body = new BasicXmlMarshalable(
			BODY_ELEMENT_NAME,
			EMPTY_VALUE,
			SOAP_NSPAIR);

	(*root)->addXmlChild(body);

	XmlMarshalable *el = new BasicXmlMarshalable(elname,
			EMPTY_VALUE,
			IFMAP_NSPAIR);
	
	body->addXmlChild(el);

	return el;

}

static void
addXmlSessionId(XmlMarshalable *el, IfmapRequest *const req)
{
	el->addXmlAttribute(STRP(SESSIONID_ATTR_NAME,
				req->getSessionId()));
}

template<class T>
T *checked_cast(Request *r)
{
	T* ret = dynamic_cast<T*>(r);

	// FIXME: Exception?
	if (!ret)
		throw "Wrong type for handler";

	return ret;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN NEWSESSION REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(NewSessionRequest, req)
{
	NewSessionRequest *nsr = checked_cast<NewSessionRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable* xNsr = getFor(NEWSESSION_ELEMENT_NAME, &root);
	int mprs = nsr->getMaxPollResultSize();
	
	if (mprs != NO_MAX_POLL_RES_SIZE) {
		stringstream ss;
		ss << mprs;
		xNsr->addXmlAttribute(STRP(MAX_POLL_RES_SIZE_ATTR_NAME,
					ss.str()));
	}

	return root;
}

IFMAP2C_RH_FROMXML_DEF(NewSessionRequest, xml)
{
	string pId, sId;
	int maxPollResSize = NO_MAX_POLL_RES_SIZE;
	bool pIdFound = false, sIdFound = false;
	XmlMarshalable *nsr 
		= findIfmapResultChecked(NEWSESSIONRESULT_ELEMENT_NAME, xml);
	CSTRPLISTIT it = nsr->getXmlAttributes().begin();
	CSTRPLISTIT end = nsr->getXmlAttributes().end();

	for (/* see above */; it != end; it++) {
		if (cmpAttrName(*it, PUBLISHERID_ATTR_NAME)) {
			pId = it->second;
			pIdFound = true;
		} else if (cmpAttrName(*it, SESSIONID_ATTR_NAME)) {
			sId = it->second;
			sIdFound = true;
		} else if (cmpAttrName(*it, MAX_POLL_RES_SIZE_ATTR_NAME)) {
			stringstream ss(it->second);
			if (!(ss >> maxPollResSize))
				throw RequestHandlerError("Invalid "
						"max-poll-result-size");
		} else {
			// unknown attribute
		}
	}

	if (!pIdFound)
		throw RequestHandlerError("PublisherId not found");

	if(!sIdFound)
		throw RequestHandlerError("SessionId not found");
	
	return new NewSessionResult(sId, pId, maxPollResSize);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END NEWSESSION REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN ENDSESSION REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(EndSessionRequest, req)
{
	EndSessionRequest *esr = checked_cast<EndSessionRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable* xEsr = getFor(ENDSESSION_ELEMENT_NAME, &root);

	addXmlSessionId(xEsr, esr);

	return root;
}


IFMAP2C_RH_FROMXML_DEF(EndSessionRequest, xml)
{
	(void)findIfmapResultChecked(ENDSESSIONRESULT_ELEMENT_NAME, xml);
	return NULL;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END ENDSESSION REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN RENEWSESSION REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(RenewSessionRequest, req)
{
	RenewSessionRequest *rnsr = checked_cast<RenewSessionRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable* xRnsr = getFor(RENEWSESSION_ELEMENT_NAME, &root);

	addXmlSessionId(xRnsr, rnsr);

	return root;
}

IFMAP2C_RH_FROMXML_DEF(RenewSessionRequest, xml)
{
	(void)findIfmapResultChecked(RENEWSESSIONRESULT_ELEMENT_NAME, xml);
	return NULL;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END RENEWSESSION REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN PURGE PUBLISHER REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(PurgePublisherRequest, req)
{
	PurgePublisherRequest *ppr = 
		checked_cast<PurgePublisherRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable* xPpr = getFor(PURGEPUBLISHER_ELEMENT_NAME, &root);

	addXmlSessionId(xPpr, ppr);
	xPpr->addXmlAttribute(STRP(STRP(PUBLISHERID_ATTR_NAME,
					ppr->getPublisherId())));

	return root;
}

IFMAP2C_RH_FROMXML_DEF(PurgePublisherRequest, xml)
{
	(void)findIfmapResultChecked(PURGEPUBLISHERRECV_ELEMENT_NAME, xml);
	return NULL;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END PURGE PUBLISHER REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

	


template<class T>
T *checked_cast(PublishElement *pe)
{
	T* ret = dynamic_cast<T*>(pe);

	// FIXME: Exception?
	if (!ret)
		throw "Wrong type for handler";

	return ret;
}

static XmlMarshalable *
getXmlPeFor(const string& name)
{
	return new BasicXmlMarshalable(name,
			EMPTY_VALUE,
			NO_NSPAIR);
}

static void
addXmlIdentifiers(XmlMarshalable *const to, IdentifierHolder *const from)
{
	Identifier *i1 = from->getIdentifier1();
	Identifier *i2 = from->getIdentifier2();

	if (i1)
		to->addXmlChild(identDispatch.toXml(i1));

	if (i2)
		to->addXmlChild(identDispatch.toXml(i2));

}

static void
addXmlMetadata(XmlMarshalable *const to, 
		IdentifierMetadataHolder *const from)
{
	XmlMarshalable *meta;
	CXMLMLISTIT it, end;

	// Bail out if we have no metadata... would be weird though.
	if (from->getMetadata().size() == 0)
		return;

	meta = new BasicXmlMarshalable(METADATA_LIST_ELEMENT_NAME,
			EMPTY_VALUE, NO_NSPAIR);

	to->addXmlChild(meta);

	it = from->getMetadata().begin();
	end = from->getMetadata().end();

	for (/* */; it != end; it++) {
		meta->addXmlChild((*it)->clone());
	}
}


static void
addXmlFilterString(XmlMarshalable *const to, PublishDelete *const from)
{
	if (from->getFilter() == FILTER_MATCH_ALL)
		return;

	to->addXmlAttribute(STRP(PUBLISH_DELETE_FILTER_ATTR_NAME,
				from->getFilter()));
}

static void
addXmlLifeTime(XmlMarshalable *to, PublishUpdate *const from)
{
	to->addXmlAttribute(STRP(PUBLISH_UPDATE_LIFET_ATTR_NAME,
				from->getLifeTimeString()));
}

static XmlMarshalable *
peToXml(PublishElement *const pe)
{
	PublishDelete *pd = NULL;
	PublishUpdate *pu = NULL;
	PublishNotify *pn = NULL;
	XmlMarshalable *xPe = NULL;
	
	if (typeid(*pe) == typeid(PublishDelete)) {
		xPe = getXmlPeFor(PUBLISH_DELETE_ELEMENT_NAME);
		pd = checked_cast<PublishDelete>(pe);
	} else if (typeid(*pe) == typeid(PublishNotify)) {
		xPe = getXmlPeFor(PUBLISH_NOTIFY_ELEMENT_NAME);
		pn = checked_cast<PublishNotify>(pe);
	} else if (typeid(*pe) == typeid(PublishUpdate)) {
		xPe = getXmlPeFor(PUBLISH_UPDATE_ELEMENT_NAME);
		pu = checked_cast<PublishUpdate>(pe);
	} else {
		throw RequestHandlerError("Unknown Publish Element");
	}

	addXmlIdentifiers(xPe, checked_cast<IdentifierHolder>(pe));
	
	if (pd) addXmlFilterString(xPe, pd);
	if (pu) {
		addXmlMetadata(xPe, pu);
		addXmlLifeTime(xPe, pu);
	}
	if (pn) addXmlMetadata(xPe, pn);

	return xPe;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN PUBLISH REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

IFMAP2C_RH_TOXML_DEF(PublishRequest, req)
{
	PublishRequest *pr = checked_cast<PublishRequest>(req);
	XmlMarshalable *root;
	CPELISTIT it, end;
	XmlMarshalable *xPr = getFor(PUBLISH_ELEMENT_NAME, &root);

	addXmlSessionId(xPr, pr);
	
	addXmlNamespaceDefs(xPr, pr);

	it = pr->getPublishElements().begin();
	end = pr->getPublishElements().end();

	for (/* */; it != end; it++)
		xPr->addXmlChild(peToXml(*it));

	return root;
}

IFMAP2C_RH_FROMXML_DEF(PublishRequest, xml)
{
	(void)findIfmapResultChecked(PUBLISHRECV_ELEMENT_NAME, xml);
	return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END PUBLISH REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

static void
addSearchParameters(XmlMarshalable *const el, SearchParam *const sp)
{
	const char *const matchLinksFilter = sp->getMatchLinksFilter();
	const int maxDepth = sp->getMaxDepth();
	const char *const resultFilter = sp->getResultFilter();
	const int maxResultSize = sp->getMaxResultSize();
	const char *const terminalIdent = sp->getTerminalIdentifiers();
	Identifier *const start = sp->getStartIdentifier();
	
	if (matchLinksFilter)
		el->addXmlAttribute(STRP(SEARCH_MATCH_LINKS_ATTR_NAME,
					matchLinksFilter));

	if (maxDepth >= 0) {
		stringstream ss;
		ss << maxDepth;
		el->addXmlAttribute(STRP(SEARCH_MAX_DEPTH_ATTR_NAME,
					ss.str()));
	}

	if (resultFilter)
		el->addXmlAttribute(STRP(SEARCH_RESULT_FILTER_ATTR_NAME,
					resultFilter));

	if (maxResultSize >= 0) {
		stringstream ss;
		ss << maxResultSize;
		el->addXmlAttribute(STRP(SEARCH_MAX_SIZE_ATTR_NAME,
					ss.str()));
	}

	if (terminalIdent) {
		el->addXmlAttribute(STRP(SEARCH_TERM_IDENT_ATTR_NAME,
					terminalIdent));
	}
		
	el->addXmlChild(identDispatch.toXml(start));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN SEARCH REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(SearchRequest, req)
{
	SearchRequest *sr = checked_cast<SearchRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable *xSr = getFor(SEARCH_ELEMENT_NAME, &root);

	addXmlSessionId(xSr, sr);
	addXmlNamespaceDefs(xSr, sr);

	addSearchParameters(xSr, sr);
	return root;
}

static string
findSearchResultName(XmlMarshalable *const sr)
{
	CSTRPLISTIT it = sr->getXmlAttributes().begin();
	CSTRPLISTIT end = sr->getXmlAttributes().end();

	for (/* */; it != end; it++)
		if (cmpAttrName(*it, SEARCHRESULT_NAME_ATTR_NAME))
			return (*it).second;
	return "";
}

static ResultItem*
makeResultItem(XmlMarshalable *const xRi)
{

	Identifier *i1 = NULL, *i2 = NULL, *tmpIdent;
	CXMLMLISTIT it, end, mit, mend;
	XMLMLIST mlist;
	CXMLMLIST childs = xRi->getXmlChildren();
	int nChildren = childs.size();

	if (nChildren > 3 || nChildren < 1)
		throw RequestHandlerError("resultItem has wrong number"
				" of elements");

	it = childs.begin();
	end = childs.end();

	try {

		for (/* see above */; it != end; it++) {
			if (cmpName(*it, METADATA_LIST_ELEMENT_NAME,
					METADATA_LIST_ELEMENT_HREF)) {
				mit = (*it)->getXmlChildren().begin();
				mend = (*it)->getXmlChildren().end();
				for (/* */; mit != mend; mit++)
					mlist.push_back(*mit);
				(*it)->clearXmlChildren();
			} else {
				tmpIdent = identDispatch.fromXml(*it);
				if (!i1)
					i1 = tmpIdent;
				else if (!i2)
					i2 = tmpIdent;
				else
					throw new RequestHandlerError(
							"Too many "
							"Identifier in "
							"resultItem");
			}
		}
	} catch (...) {

		// if we have problems, make sure we do
		// not loose the list...
		mit = (*it)->getXmlChildren().begin();
		mend = (*it)->getXmlChildren().end();
		
		for (/* */; mit != mend; mit++)
			delete *mit;
	}

	return new ResultItem(i1, i2, mlist);
}

static SearchResult*
makeSearchResult(XmlMarshalable *const sr)
{
	SearchResult *ret = NULL;
	std::string name = findSearchResultName(sr);
	CXMLMLISTIT it = sr->getXmlChildren().begin();
	CXMLMLISTIT end = sr->getXmlChildren().end();
	
	ret = new SearchResult(name);
	
	try {
		for (/* */; it != end; it++)
			ret->addResultItem(makeResultItem((*it)));
	} catch (...) {
		// cleanup
		delete ret;
		throw;
	}

	return ret;
}

IFMAP2C_RH_FROMXML_DEF(SearchRequest, xml)
{
	return makeSearchResult(
		findIfmapResultChecked(SEARCHRESULT_ELEMENT_NAME, xml));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END SEARCH REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

template<class T>
T *checked_cast(SubscribeElement *se)
{
	T* ret = dynamic_cast<T*>(se);

	// FIXME: Exception?
	if (!ret)
		throw "Wrong type for handler";

	return ret;
}

static void
addXmlSubName(XmlMarshalable *const to, SubscribeElement *const from)
{
	to->addXmlAttribute(STRP(SUBSCRIBE_UPDATE_NAME_ATTR_NAME, from->getName()));

}

static XmlMarshalable *
seToXml(SubscribeElement *const se)
{
	SubscribeUpdate *su = NULL;
	SubscribeDelete *sd = NULL;
	XmlMarshalable *xSe;
	
	if (typeid(*se) == typeid(SubscribeDelete)) {
		xSe = getXmlPeFor(SUBSCRIBE_DELETE_ELEMENT_NAME);
		sd = checked_cast<SubscribeDelete>(se);
	} else if (typeid(*se) == typeid(SubscribeUpdate)) {
		xSe = getXmlPeFor(SUBSCRIBE_UPDATE_ELEMENT_NAME);
		su = checked_cast<SubscribeUpdate>(se);
	} else {
		throw new RequestHandlerError("Unknown Subscribe Element");
	}

	addXmlSubName(xSe, se);

	if (su)
		addSearchParameters(xSe, su);

	return xSe;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN SUBSCRIBE REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(SubscribeRequest, req)
{
	SubscribeRequest *sr = checked_cast<SubscribeRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable *xSr = getFor(SUBSCRIBE_ELEMENT_NAME, &root);

	addXmlSessionId(xSr, sr);
	addXmlNamespaceDefs(xSr, sr);
	
	CSELISTIT it, end;
	it = sr->getSubscribeElements().begin();
	end = sr->getSubscribeElements().end();

	for (/* */; it != end; it++)
		xSr->addXmlChild(seToXml(*it));

	return root;
}

IFMAP2C_RH_FROMXML_DEF(SubscribeRequest, xml)
{
	(void)findIfmapResultChecked(SUBSCRIBERECV_ELEMENT_NAME, xml);
	return NULL;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END SUBSCRIBE REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// BEGIN POLL REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
IFMAP2C_RH_TOXML_DEF(PollRequest, req)
{
	PollRequest *pr = checked_cast<PollRequest>(req);
	XmlMarshalable *root;
	XmlMarshalable* xPr = getFor(POLLREQUEST_ELEMENT_NAME, &root);

	addXmlSessionId(xPr, pr);

	return root;
}

static PollResult *
makePollResult(XmlMarshalable *xPr)
{
	CXMLMLISTIT it = xPr->getXmlChildren().begin();
	CXMLMLISTIT end = xPr->getXmlChildren().end();
	ErrorResult *er = NULL;
	SearchResult *sr;
	PollResult *ret = new PollResult();

	try {
		for (/* see above */; it != end; ++it) {
			if (cmpName(*it,POLLRESULT_SEARCH_ELEMENT_NAME)) {
				sr = makeSearchResult(*it);
				ret->addSearchResult(sr);
			} else if (cmpName(*it, POLLRESULT_UPDATE_ELEMENT_NAME)) {
				sr = makeSearchResult(*it);
				ret->addUpdateResult(sr);
			} else if (cmpName(*it, POLLRESULT_DELETE_ELEMENT_NAME)) {
				sr = makeSearchResult(*it);
				ret->addDeleteResult(sr);
			} else if (cmpName(*it, POLLRESULT_NOTIFY_ELEMENT_NAME)) {
				sr = makeSearchResult(*it);
				ret->addNotifyResult(sr);
			} else if (cmpName(*it, POLLRESULT_ERROR_ELEMENT_NAME)) {
				er = createErrorResult(*it);
				ret->addErrorResult(er);
			} else {
				throw RequestHandlerError("PollResult has"
						" unknown element");
			}
		}
	} catch (...) {
		if (ret)
			delete ret;
		throw;
	}
	
	return ret;
}

IFMAP2C_RH_FROMXML_DEF(PollRequest, xml)
{
	XmlMarshalable *response = findIfmapResponseChecked(xml);
	XmlMarshalable *pollResult = NULL;
	
	throwIfError(response);

	pollResult = findChild(response, STRP(POLLRESULT_ELEMENT_NAME,
				NO_HREF));

	// No poll result, there might be an endSessionResult ;
	if (!pollResult) {
		if (findChild(response, 
			STRP(ENDSESSIONRESULT_ELEMENT_NAME, NO_HREF))) {
			throw EndSessionResult();
		} else {
			throw RequestHandlerError("UHHHH Really bad:(");
		}
	} 

	return makePollResult(pollResult);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	
// END POLL REQUEST
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

} // namespace
