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

#include "requests.h"
#include "typedefs.h"

using namespace std;

namespace ifmap2c {

NewSessionRequest*
Requests::createNewSessionReq(const string& maxPollResSize)
{
	return NewSessionRequest::createNewSessionRequest(maxPollResSize);
}



EndSessionRequest*
Requests::createEndSessionReq(void)
{
	return EndSessionRequest::createEndSessionRequest();
}

RenewSessionRequest *
Requests::createRenewSessionReq(void)
{
	return RenewSessionRequest::createRenewSessionRequest();
}

PublishRequest*
Requests::createPublishReq(const std::list<SubPublish *>& reqs)
{
	return PublishRequest::createPublishRequest(reqs);
}



PublishRequest*
Requests::createPublishReq(SubPublish *const  req)
{
	return PublishRequest::createPublishRequest(req);
}



PublishUpdate*
Requests::createPublishUpdate(const XMLMLIST& metadata,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2)
{
	return PublishUpdate::createPublishUpdate(metadata, i1, lTime, i2);
}



PublishUpdate*
Requests::createPublishUpdate(XmlMarshalable *const metadata,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2)
{
	return PublishUpdate::createPublishUpdate(metadata, i1, lTime, i2);
}



PublishDelete *
Requests::createPublishDelete(const char *const filter,
		Identifier *const i1,
		Identifier *const i2)
{
	return PublishDelete::createPublishDelete(filter, i1, i2);
}

PublishNotify *
Requests::createPublishNotify(const XMLMLIST& metadataList,
		Identifier *const i1,
		Identifier *const i2)
{
	return PublishNotify::createPublishNotify(metadataList, i1, i2);
}

PublishNotify *
Requests::createPublishNotify(XmlMarshalable *const metadata,
		Identifier *const i1,
		Identifier *const i2)
{
	return PublishNotify::createPublishNotify(metadata, i1, i2);
}



SearchRequest*
Requests::createSearchReq(const char *const matchLinksFilter,
		const int maxDepth,
		const char *const resultFilter,
		const int maxResultSize,
		Identifier *const i1)
{
	return createSearchReq(matchLinksFilter, maxDepth, 
			resultFilter, maxResultSize,
			SEARCH_NO_TERMINAL_IDENTIFIERS, i1);
}
SearchRequest *
Requests::createSearchReq(const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1)
{
	return SearchRequest::createSearchRequest(matchLinksFilter,
			maxDepth, resultFilter, maxResultSize, 
			terminalIdent, i1);
}


SubscribeRequest *
Requests::createSubscribeReq(const std::list<SubSubscribe *> srList)
{
	return SubscribeRequest::createSubscribeRequest(srList);
}

SubscribeRequest *
Requests::createSubscribeReq(SubSubscribe *const sr)
{
	return SubscribeRequest::createSubscribeRequest(sr);
}

SubscribeUpdate *
Requests::createSubscribeUpdate(std::string const& name,
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
Requests::createSubscribeDelete(const string& name)
{
	return SubscribeDelete::createSubscribeDelete(name);
}

PollRequest *
Requests::createPollReq(void)
{
	return PollRequest::createPollRequest();
}

PurgePublisherRequest *
Requests::createPurgePublisherReq(const string& publisherId)
{
	return PurgePublisherRequest::createPurgePublisherRequests(publisherId);
}

} // namespace
