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

#ifndef REQUESTS_H_
#define REQUESTS_H_

#include "endsessionrequest.h"
#include "identifier.h"
#include "newsessionrequest.h"
#include "pollrequest.h"
#include "publishrequest.h"
#include "purgepublisherrequest.h"
#include "renewsessionrequest.h"
#include "searchrequest.h"
#include "subscriberequest.h"
#include "xmlmarshalable.h"

namespace ifmap2c {

/**
 * Class to be used by a user of the library to create different kinds
 * off request objects.
 */
class Requests {

public:
	/**
	 * Used internally, not to be used by users of the library.
	 */
	static NewSessionRequest *createNewSessionReq(
			const int maxPollResSize = NO_MAX_POLL_RES_SIZE);

	/**
	 * Used internally, not to be used by users of the library.
	 */
	static EndSessionRequest *createEndSessionReq(void);

	/**
	 * Used internally, not to be used by users of the library.
	 */
	static RenewSessionRequest *createRenewSessionReq(void);

	/**
	 * Create a publish request object.
	 *
	 * @param reqs list of PublishElement objects
	 * 	(PublishUpdate, PublishDelete or PublishNotify)
	 */
	static PublishRequest *createPublishReq(
			const std::list<PublishElement *>& reqs);

	/**
	 * Create a publish request object.
	 *
	 * @param req PublishElement object
	 *        (PublishUpdate, PublishDelete or PublishNotify)
	 */
	static PublishRequest *createPublishReq(
			PublishElement *const  req);

	/**
	 * Create a publish update object.
	 *
	 * @param mdlist a list of metadata objects associated with this
	 *        update
	 * @param i1 first identifier object
	 * @param ltime lifetime of the metadata
	 * @param i2 second identifier object (link)
	 */
	static PublishUpdate *createPublishUpdate(
			const std::list<XmlMarshalable *>& mdlist,
			Identifier *const i1,
			LifeTimeType lTime = session,
			Identifier *const i2 = NULL);

	/**
	 * Create a publish update object containing a link.
	 *
	 * @param mdlist a list of metadata objects associated with this
	 *        update
	 * @param i1 first identifier object
	 * @param i2 second identifier object
	 * @param ltime lifetime of the metadata
	 */
	static PublishUpdate *createPublishUpdate(
			const std::list<XmlMarshalable *>& mdlist,
			Identifier *const i1,
			Identifier *const i2,
			LifeTimeType lTime = session);

	/**
	 * Create a publish update object.
	 *
	 * @param md a single metadata object associated with this update
	 * @param i1 first identifier object
	 * @param ltime lifetime of the metadata
	 * @param i2 second identifier object (link)
	 */
	static PublishUpdate *createPublishUpdate(
			XmlMarshalable *const md,
			Identifier *const i1,
			LifeTimeType ltime = session,
			Identifier *const i2 = NULL);
	
	/**
	 * Create a publish update object containing a link.
	 *
	 * @param md a single metadata object associated with this update
	 * @param i1 first identifier object
	 * @param ltime lifetime of the metadata
	 * @param i2 second identifier object (link)
	 */
	static PublishUpdate *createPublishUpdate(
			XmlMarshalable *const md,
			Identifier *const i1,
			Identifier *const i2,
			LifeTimeType ltime = session);


	/**
	 * Create a publish delete object.
	 *
	 * If you create a PublishDelete object, be aware that if
	 * namespace prefixes are used in the filter, e.g. "meta:role",
	 * the corresponding namespace must be defined somewhere in
	 * the request.
	 * Use addXmlNamespaceDefinition() on the returned object to
	 * achieve this.
	 *
	 * @param filter filterstring, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param i1 first identifier object
	 * @param i2 first identifier object (link)
	 */
	static PublishDelete *createPublishDelete(
			const char *const filter,
			Identifier *const i1,
			Identifier *const i2 = NULL);
	/**
	 * Create a publish notify object.
	 *
	 * @param mdlist a list of metadata objects associated with this
	 *        update
	 * @param i1 first identifier object
	 * @param i2 second identifier object (link)
	 */
	static PublishNotify *createPublishNotify(
			const std::list<XmlMarshalable *>& mdlist,
			Identifier *const i1,
			Identifier *const i2 = NULL);

	/**
	 * Create a publish notify object.
	 *
	 * @param md a single metadata object associated with this update
	 * @param i1 first identifier object
	 * @param i2 second identifier object (link)
	 */
	static PublishNotify *createPublishNotify(
			XmlMarshalable *const metadata,
			Identifier *const i1,
			Identifier *const i2 = NULL);

	/**
	 * Create a search request object.
	 *
	 * @param matchLinksFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxDepth maximum depth, use SEARCH_NO_MAX_DEPTH to not
	 *        add a maximum depth
	 * @param resultFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxResultSize maximum result size, use
	 *        SEARCH_NO_MAX_RESULT_SIZE to not add a maximum result
	 *        size
	 * @param i1 identifier object where the search will start
	 */
	static SearchRequest *createSearchReq(
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			Identifier *const i1);
	/**
	 * Create a search request object.
	 *
	 * @param matchLinksFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxDepth maximum depth, use SEARCH_NO_MAX_DEPTH to not
	 *        add a maximum depth
	 * @param resultFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxResultSize maximum result size, use
	 *        SEARCH_NO_MAX_RESULT_SIZE to not add a maximum result
	 *        size
	 * @param terminalIdent terminal identifiers as string, separated
	 *        by commas, use SEARCH_NO_TERMINAL_IDENTIFIERS to not add
	 *        the terminal identifiers attribute
	 * @param i1 identifier object where the search will start
	 */
	static SearchRequest *createSearchReq(
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1);

	/**
	 * Create a subscribe request object.
	 *
	 * @param srList list of SubscribeElement objects
	 * 		 (SubscribeUpdate, SubscribeDelete)
	 */
	static SubscribeRequest *createSubscribeReq(
			const std::list<SubscribeElement *> srList);

	/**
	 * Create a subscribe request object.
	 *
	 * @param sub SubscribeElement object
	 * 	      (SubscribeUpdate, SubscribeDelete)
	 */
	static SubscribeRequest *createSubscribeReq(
			SubscribeElement *const sub);

	/**
	 * Create a subscribe update object.
	 *
	 * @param name name of the subscription
	 * @param matchLinksFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxDepth maximum depth, use SEARCH_NO_MAX_DEPTH to not
	 *        add a maximum depth
	 * @param resultFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxResultSize maximum result size, use
	 *        SEARCH_NO_MAX_RESULT_SIZE to not add a maximum result
	 *        size
	 * @param i1 identifier object where the search will start
	 */
	static SubscribeUpdate *createSubscribeUpdate(
			std::string const& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			Identifier *const i1);
	
	/**
	 * Create a subscribe update object.
	 *
	 * @param name name of the subscription
	 * @param matchLinksFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxDepth maximum depth, use SEARCH_NO_MAX_DEPTH to not
	 *        add a maximum depth
	 * @param resultFilter filter string, use FILTER_MATCH_ALL or
	 *        FILTER_MATCH_NOTHING, when this behaviour is wanted
	 * @param maxResultSize maximum result size, use
	 *        SEARCH_NO_MAX_RESULT_SIZE to not add a maximum result
	 *        size
	 * @param terminalIdent terminal-identifier-type as string, use
	 *        SEARCH_NO_TERMINAL_IDENTIFIERS if this field should not
	 *        be set.
	 * @param i1 identifier object where the search will start
	 */
	static SubscribeUpdate *createSubscribeUpdate(
			std::string const& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1);
	/**
	 * Create a subscribe delete object.
	 *
	 * @param name name of the subscription
	 */
	static SubscribeDelete *createSubscribeDelete(
			std::string const& name);

	/**
	 * Used internally, not to be used by users of the library.
	 */
	static PollRequest *createPollReq(void);

	/**
	 * Used internally, not to be used by users of the library.
	 */
	static PurgePublisherRequest *createPurgePublisherReq(
			std::string const& publisherId);
};

} // namespace
#endif /* REQUESTS_H_ */
