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

#ifndef REQUESTS_H_
#define REQUESTS_H_

#include "xmlmarshalable.h"
#include "identifiers.h"
#include "newsessionrequest.h"
#include "publishrequest.h"
#include "endsessionrequest.h"
#include "publishupdate.h"
#include "publishdelete.h"
#include "publishnotify.h"
#include "subpublish.h"
#include "searchrequest.h"
#include "subscriberequest.h"
#include "subsubscribe.h"
#include "subscribeupdate.h"
#include "subscribedelete.h"
#include "pollrequest.h"
#include "renewsessionrequest.h"

namespace LIBRARY_NAMESPACE {

/*
 * A factory class to create all kinds of requests.
 */
class Requests {

public:
	static NewSessionRequest *createNSR(
			const std::string& maxPollResSize = "");

	static EndSessionRequest *createESR(void);

	static RenewSessionRequest *createRNSR(void);

	static PublishRequest *createPR(const std::list<SubPublish *>& reqs);

	static PublishRequest *createPR(SubPublish *const  reqs);

	static PublishUpdate *createPU(
			const std::list<XmlMarshalable *>& metadata,
			Identifier *const i1,
			LifeTimeType lTime = session,
			Identifier *const i2 = NULL);

	static PublishUpdate *createPU(XmlMarshalable *const metadata,
			Identifier *const i1,
			LifeTimeType lTime = session,
			Identifier *const i2 = NULL);

	static PublishDelete *createPD(const char *const filter,
			Identifier *const i1,
			Identifier *const i2 = NULL);

	static PublishNotify *createPN(
			const std::list<XmlMarshalable *>& metadataList,
			Identifier *const i1,
			Identifier *const i2 = NULL);

	static PublishNotify *createPN(XmlMarshalable *const metadata,
			Identifier *const i1,
			Identifier *const i2 = NULL);


	static SearchRequest *createSR(const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			Identifier *const i1);


	static SubscribeRequest *createSubR(
			const std::list<SubSubscribe *> srList);

	static SubscribeRequest *createSubR(SubSubscribe *const sr);

	static SubscribeUpdate *createSubU(std::string const& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			Identifier *const i1);

	static SubscribeDelete *createSubD(std::string const& name);

	static PollRequest *createPollR(std::string const& name);
};
} // namespace
#endif /* REQUESTS_H_ */
