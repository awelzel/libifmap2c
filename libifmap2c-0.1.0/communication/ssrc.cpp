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

#include "ssrc.h"
#include "arc.h"

#include <iostream>

using namespace std;

namespace ifmap2c {

SSRC::SSRC(const string& url,
		const string& user,
		const string& pass,
		const string& capath) :
		IfmapCommunication(url, user, pass, capath)
{ }

SSRC::SSRC(const string& url,
		const string& mykey,
		const string& mykeypw,
		const string& mycert,
		const string& capath) :
		IfmapCommunication(url, mykey, mykeypw, mycert, capath)
{  }



SSRC::~SSRC()
{ /* cleanup is done in superclass */ }



SSRC *
SSRC::createSSRC(const string& url,
		const string& user,
		const string& pass,
		const string& capath)
{
	return new SSRC(url, user, pass, capath);
}



SSRC *
SSRC::createSSRC(const string& url,
		const string& mykey,
		const string& mykeypw,
		const string& mycert,
		const string& capath)
{
	return new SSRC(url, mykey, mykeypw, mycert, capath);
}



ARC *
SSRC::getARC(void) {
	ARC *ret = NULL;

	if (_basicAuth)
		ret = new ARC(_url, _userName, _password, _caPath, this);
	else
		ret = new ARC(_url, _keyFile, _certFile, _password, _caPath, this);

	return ret;
}


void
SSRC::newSession(const string& maxPollResSize)
{
	NewSessionRequest *nsreq = Requests::createNewSessionReq(maxPollResSize);
	XmlMarshalable *reply = NULL;
	NewSessionResult *nsres = NULL;

	try {
		reply = processMessage(nsreq);
		nsres = ResponseParser::createNewSessionResult(reply);
	} catch (...) {
		// free the memory
		if (reply)
			delete reply;
		if (nsreq)
			delete nsreq;
		// throw exception up
		throw;
	}

	_currentSessionId = nsres->getSessionId();
	_currentPublisherId = nsres->getPublisherId();
	_currentMaxPollResSize = nsres->getMaxPollResultSize();
	delete nsreq;
	delete reply;
	delete nsres;
}



void
SSRC::endSession(const string& sId)
{
	EndSessionRequest *esreq = Requests::createEndSessionReq();
	XmlMarshalable *reply = NULL;

	if (sId.length() > 0)
		setSessionId(esreq, sId);

	try {
		reply = processMessage(esreq);
		ResponseParser::checkEndSessionResult(reply);
	} catch (...) {
		delete esreq;
		delete reply;
		throw;
	}
	delete esreq;
	delete reply;
}



void
SSRC::publish(PublishRequest *const pr, const string& sId)
{
	XmlMarshalable *reply = NULL;

	if (sId.length() > 0)
		setSessionId(pr, sId);

	try {
		reply = processMessage(pr);
		ResponseParser::checkPublishReceived(reply);
	} catch (...) {
		if (reply)
			delete reply;
		throw;
	}
	delete reply;
}



SearchResult *
SSRC::search(SearchRequest *const sr, const string& sId)
{
	XmlMarshalable *reply  = NULL;
	SearchResult *sres = NULL;

	if (sId.length() > 0)
		setSessionId(sr, sId);

	try {
		reply = processMessage(sr);
		sres = ResponseParser::createSearchResult(reply);
	} catch (...) {
		if (reply)
			delete reply;
		throw;
	}

	delete reply;
	return sres;
}

void
SSRC::subscribe(SubscribeRequest *const sr, const string& sId)
{
	XmlMarshalable *reply = NULL;

	if (sId.length() > 0)
		setSessionId(sr, sId);

	try {
		reply = processMessage(sr);
		ResponseParser::checkSubscribeReceived(reply);
	} catch (...) {
		if (reply)
			delete reply;
		throw;
	}

	delete reply;
}

void
SSRC::renewSession(const string& sId)
{
	RenewSessionRequest *rsr = Requests::createRenewSessionReq();
	XmlMarshalable *reply = NULL;

	if (sId.length() > 0)
		setSessionId(rsr, sId);

	try {
		reply = processMessage(rsr);
		ResponseParser::checkRenewSessionResult(reply);
	} catch (...) {
		if (rsr)
			delete rsr;
		if (reply)
			delete reply;
		throw;
	}
	delete rsr;
	delete reply;
}



void
SSRC::purgePublisher(const string& sId, const string& pId)
{
	// take either the given or the actual publisherId for this request.
	string pubId = (pId.length() > 0) ? pId : getPublisherId();

	PurgePublisherRequest *ppr = Requests::createPurgePublisherReq(pubId);
	XmlMarshalable *reply = NULL;

	if (sId.length() > 0)
		setSessionId(ppr, sId);

	try {
		reply = processMessage(ppr);
		ResponseParser::checkPurgePublisherReceived(reply);
	} catch (...) {
		if (ppr) delete ppr;
		if (reply) delete reply;
		throw;
	}
	delete ppr;
	delete reply;
}



const string&
SSRC::getSessionId(void) const
{
	return _currentSessionId;
}



const string&
SSRC::getPublisherId(void) const
{
	return _currentPublisherId;
}



const string&
SSRC::getMaxPollResultSize(void) const
{
	return _currentMaxPollResSize;
}

} // namespace
