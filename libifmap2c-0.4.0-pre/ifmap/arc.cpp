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

using namespace std;

namespace ifmap2c {


ARC::ARC(const string& url,
		const string& user,
		const string& pass,
		const string& capath,
		SSRC *const ssrc) :
		IfmapChannel(url, user, pass, capath), _ssrc(ssrc)
{ }

ARC::ARC(const string& url,
		const string& mykey,
		const string& mykeypw,
		const string& mycert,
		const string& capath,
		SSRC *const ssrc) :
		IfmapChannel(url, mykey, mykeypw, mycert, capath),
		_ssrc(ssrc)
{ }


ARC::~ARC() {
	// TODO Auto-generated destructor stub
}


PollResult *
ARC::poll(const string& sId)
{
	string sessionId = (sId.length() > 0) ? sId : getSessionId();
	PollRequest *pollReq = Requests::createPollReq();
	Result *res = NULL;
	PollResult *ret = NULL;
	pollReq->setSessionId(sessionId);
	
	try {
		Result *res = _xmlCommunication->genericRequest(pollReq);
		ret = dynamic_cast<PollResult *>(res);
		if (!ret)
			throw RequestHandlerError("No PollResult"
				" in response to a poll");

	} catch (...) {
		if (res)
			delete res;

		if (pollReq)
			delete pollReq;

		throw;
	}

	delete pollReq;

	return ret;
}

const string&
ARC::getSessionId(void) const
{
	return _ssrc->getSessionId();
}


const string&
ARC::getPublisherId(void) const
{
	return _ssrc->getPublisherId();
}

int
ARC::getMaxPollResultSize(void) const
{
	return _ssrc->getMaxPollResultSize();
}

} // namespace
