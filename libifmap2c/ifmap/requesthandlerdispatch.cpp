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
#include "ifmaprequest.h"
#include "requests.h"

#include <iostream>

/*
 * TODO: Somebody has to tell me something better than dispatching
 *       on the name value of the type_info, please.
 */
using namespace std;

namespace ifmap2c {

class InitIfmapBaseRequestHandlers {

public:

	InitIfmapBaseRequestHandlers() {
		IfmapRequestHandlerDispatch::handlers = 
			new list<RequestHandler *>(baseHandlers());

	}
	
	~InitIfmapBaseRequestHandlers() {
		list<RequestHandler *>::const_iterator it, end;
		it = IfmapRequestHandlerDispatch::handlers->begin();
		end = IfmapRequestHandlerDispatch::handlers->end();
		
		for (/* */; it != end; it++)
			delete *it;

		IfmapRequestHandlerDispatch::handlers->clear();
		delete IfmapRequestHandlerDispatch::handlers;
	}

private:
	list<RequestHandler *> baseHandlers(void)
	{
		list<RequestHandler *> ret;
		ret.push_back(IFMAP2C_RH_CREATE_CALL(NewSessionRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(EndSessionRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(RenewSessionRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(PurgePublisherRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(PublishRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(SearchRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(SubscribeRequest));
		ret.push_back(IFMAP2C_RH_CREATE_CALL(PollRequest));
		return ret;
	}
};

// force initialization of handlers
static InitIfmapBaseRequestHandlers initIfmapBaseRequestHandlers;

list<RequestHandler*> *IfmapRequestHandlerDispatch::handlers;

void
IfmapRequestHandlerDispatch::registerHandler(RequestHandler
		*const handler)
{
	handlers->push_back(handler);
}

RequestHandler *
IfmapRequestHandlerDispatch::dispatch(Request *const req) const
{
	list<RequestHandler *>::const_iterator it, end;
	it = handlers->begin();
	end = handlers->end();

	for (/* */; it != end; it++)
		if ((*it)->canHandle(req))
			return *it;

	// FIXME
	throw "NO REQUEST HANDLER FOUND";
}

} // namespace
