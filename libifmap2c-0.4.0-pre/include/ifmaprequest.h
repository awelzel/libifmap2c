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

#ifndef IFMAPREQUEST_H_
#define IFMAPREQUEST_H_

#include "request.h"

#include <list>
#include <string>
#include <typeinfo>

namespace ifmap2c {

class IfmapRequest : public Request {

public:
	virtual ~IfmapRequest() { };
	IfmapRequest(const std::string& sId = "") : _sessionId(sId) { }

	const std::string& getSessionId(void) const {
		return _sessionId;
	}
	
	void setSessionId(const std::string& sId) {
		_sessionId = sId;
	}

	void
	addXmlNamespaceDefinition(
			const std::pair<std::string, std::string>& nsDef)
	{
		_nsDefs.push_back(nsDef);

	}

	const std::list<std::pair<std::string, std::string> >& 
	getXmlNamespaceDefinitions(void)
	{
		return _nsDefs;
	}


private:
	std::string _sessionId;
	std::list<std::pair<std::string, std::string> > _nsDefs;
};


class InitIfmapBaseRequestHandlers;

class IfmapRequestHandlerDispatch : public RequestHandlerDispatch {

public:
	/**
	 * Register a new RequestHandler
	 */
	static void
	registerHandler(RequestHandler *const handler);
	
	/**
	 * Get the appropiate RequestHandler for the given Request
	 */
	RequestHandler *dispatch(Request *const req) const;

	friend class InitIfmapBaseRequestHandlers;

private:
	static std::list<RequestHandler*> *handlers;
};

} // namespace

#endif /* IFMAPREQUEST_H_*/
