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

#include "arc.h"
#include "ssrc.h"

#include <iostream>

using namespace std;

namespace ifmap2c {

SSRC::SSRC(const string& url,
		const string& user,
		const string& pass,
		const string& capath) :
		IfmapChannel(url, user, pass, capath)
{ }

SSRC::SSRC(const string& url,
		const string& mykey,
		const string& mykeypw,
		const string& mycert,
		const string& capath) :
		IfmapChannel(url, mykey, mykeypw, mycert, capath)
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
SSRC::newSession(const int maxPollResSize)
{
	NewSessionRequest *nsreq = Requests::createNewSessionReq(maxPollResSize);
	Result *res = NULL;
	NewSessionResult *nsres = NULL;
	int maxPollResSizeRecv = NO_MAX_POLL_RES_SIZE;
	
	try {
		res = _xmlCommunication->genericRequest(nsreq);

		nsres = dynamic_cast<NewSessionResult *>(res);
		
		if (!nsres)
			throw "UHM :( bad result"; //FIXME

		maxPollResSizeRecv = nsres->getMaxPollResultSize();
		
		if (maxPollResSize != NO_MAX_POLL_RES_SIZE)
			if (maxPollResSizeRecv == NO_MAX_POLL_RES_SIZE)
				throw CommunicationError("Server did not"
						" set max-poll-result-size"
						" in newSessionResult");
	} catch (...) {
		// free the memory
		if (nsreq)
			delete nsreq;
		if (nsres)
			delete nsres;
		// throw exception up
		throw;
	}

	_currentSessionId = nsres->getSessionId();
	_currentPublisherId = nsres->getPublisherId();
	_currentMaxPollResSize = maxPollResSizeRecv;
	delete nsreq;
	delete nsres;
}

void
SSRC::endSession(const string& sId)
{
	EndSessionRequest *esreq = Requests::createEndSessionReq();
	string sessionId = (sId.length() > 0) ? sId : getSessionId();
	esreq->setSessionId(sessionId);

	try {
		_xmlCommunication->genericRequest(esreq);
	} catch (...) {
		delete esreq;
		throw;
	}
	delete esreq;
/*
EndSessionRequest::EndSessionRequest() : BasicXmlMarshalable(
			ENDSESSION_ELEMENT_NAME,
			EMPTY_VALUE,
			IFMAP_NSPAIR)
{ }
*/
}



void
SSRC::publish(PublishRequest *const pr, const string& sId)
{
	string sessionId = (sId.length() > 0) ? sId : getSessionId();
	pr->setSessionId(sessionId);
	_xmlCommunication->genericRequest(pr);
}



SearchResult *
SSRC::search(SearchRequest *const sr, const string& sId)
{
	cerr << "TOOD TODO" << sId;
	throw sr;
	/*
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
	*/
}

void
SSRC::subscribe(SubscribeRequest *const sr, const string& sId)
{
	cerr << "TOOD TODO" << sId;
	throw sr;
	/*
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
	*/
}

void
SSRC::renewSession(const string& sId)
{
	RenewSessionRequest *rnsreq = Requests::createRenewSessionReq();
	string sessionId = (sId.length() > 0) ? sId : getSessionId();

	try {
		_xmlCommunication->genericRequest(rnsreq);
	} catch (...) {
		delete rnsreq;
	}
	delete rnsreq;
	/*
RenewSessionRequest::RenewSessionRequest() :
	BasicXmlMarshalable(RENEWSESSION_ELEMENT_NAME, EMPTY_VALUE,
			IFMAP_OPERATION_NSPAIR)
{ }
*/
}



void
SSRC::purgePublisher(const string& pId, const string& sId)
{
	// take either the given or the actual publisherId for this request.
	string pubId = (pId.length() > 0) ? pId : getPublisherId();
	string sessionId = (sId.length() > 0) ? sId : getSessionId();

	PurgePublisherRequest *ppr = 
		Requests::createPurgePublisherReq(pubId);
	try {
		_xmlCommunication->genericRequest(ppr);
	} catch (...) {
		delete ppr;
	}
	delete ppr;

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



int
SSRC::getMaxPollResultSize(void) const
{
	return _currentMaxPollResSize;
}

} // namespace
