/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2010, A. Welzel, <arne.welzel@gmail.com>
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

#include "requests.h"
#include "typedefs.h"

using namespace std;

namespace LIBRARY_NAMESPACE {

NewSessionRequest*
Requests::createNSR(const string& maxPollResSize)
{
	return NewSessionRequest::createNewSessionRequest(maxPollResSize);
}



EndSessionRequest*
Requests::createESR(void)
{
	return EndSessionRequest::createEndSessionRequest();
}

RenewSessionRequest *
Requests::createRNSR(void)
{
	return RenewSessionRequest::createRenewSessionRequest();
}

PublishRequest*
Requests::createPR(const std::list<SubPublish *>& reqs)
{
	return PublishRequest::createPublishRequest(reqs);
}



PublishRequest*
Requests::createPR(SubPublish *const  req)
{
	return PublishRequest::createPublishRequest(req);
}



PublishUpdate*
Requests::createPU(const CLIST& metadata,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2)
{
	return PublishUpdate::createPublishUpdate(metadata, i1, lTime, i2);
}



PublishUpdate*
Requests::createPU(XmlMarshalable *const metadata,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2)
{
	return PublishUpdate::createPublishUpdate(metadata, i1, lTime, i2);
}



PublishDelete *
Requests::createPD(const char *const filter,
		Identifier *const i1,
		Identifier *const i2)
{
	return PublishDelete::createPublishDelete(filter, i1, i2);
}

PublishNotify *
Requests::createPN(const CLIST& metadataList,
		Identifier *const i1,
		Identifier *const i2)
{
	return PublishNotify::createPublishNotify(metadataList, i1, i2);
}

PublishNotify *
Requests::createPN(XmlMarshalable *const metadata,
		Identifier *const i1,
		Identifier *const i2)
{
	return PublishNotify::createPublishNotify(metadata, i1, i2);
}



SearchRequest*
Requests::createSR(const char *const matchLinksFilter,
		const int maxDepth,
		const char *const resultFilter,
		const int maxResultSize,
		Identifier *const i1)
{
	return SearchRequest::createSearchRequest(matchLinksFilter,
			maxDepth, resultFilter, maxResultSize, i1);
}


SubscribeRequest *
Requests::createSubR(const std::list<SubSubscribe *> srList)
{
	return SubscribeRequest::createSubscribeRequest(srList);
}

SubscribeRequest *
Requests::createSubR(SubSubscribe *const sr)
{
	return SubscribeRequest::createSubscribeRequest(sr);
}

SubscribeUpdate *
Requests::createSubU(std::string const& name,
		const char *const matchLinksFilter,
		const int maxDepth,
		const char *const resultFilter,
		const int maxResultSize,
		Identifier *const i1)
{
	return SubscribeUpdate::createSubscribeUpdate(name, matchLinksFilter,
			maxDepth, resultFilter, maxResultSize, i1);
}

SubscribeDelete *
Requests::createSubD(const string& name)
{
	return SubscribeDelete::createSubscribeDelete(name);
}

PollRequest *
Requests::createPollR(const string& name)
{
	return PollRequest::createPollRequest(name);
}

} // namespace
