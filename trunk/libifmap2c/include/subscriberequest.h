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

#ifndef SUBSCRIBEREQUEST_H_
#define SUBSCRIBEREQUEST_H_
#include "searchparam.h"
#include "ifmaprequest.h"
#include <list>

namespace ifmap2c {

class SubscribeElement {

public:
	const std::string getName(void) const;

	virtual ~SubscribeElement();

protected:
	SubscribeElement(const std::string& name);

private:
	const std::string _name;
};


class SubscribeUpdate : public SubscribeElement, public SearchParam {
public:
	/*
	 * Use char* instead of std::string, in order to give the user
	 * to not specify any filter.
	 *
	 * NULL: match everything
	 * empty string: match nothing ("")
	 * else: a filter string ("meta:role")
	 *
	 * To not send a depth in the SearchRequest, set depth to -1.
	 * Equally, to not send a max-result-size in the SearchRequest,
	 * set maxResultSize to -1
	 */
	static SubscribeUpdate*
	createSubscribeUpdate(const std::string& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1);

	virtual ~SubscribeUpdate();

private:

	SubscribeUpdate(const std::string& name,
			const char *const matchLinksFilter,
			const int maxDepth,
			const char *const resultFilter,
			const int maxResultSize,
			const char *const terminalIdent,
			Identifier *const i1);
};

class SubscribeDelete : public SubscribeElement {

public:
	virtual ~SubscribeDelete();

	static SubscribeDelete *createSubscribeDelete(const std::string& name);

private:
	SubscribeDelete(const std::string& name);
};

class SubscribeRequest : public IfmapRequest {

public:
	const std::list<SubscribeElement *>&
	getSubscribeElements(void) const;

	static SubscribeRequest *createSubscribeRequest(
			const std::list<SubscribeElement *>& subList);

	static SubscribeRequest *createSubscribeRequest(
			SubscribeElement *const subreq);
	
	virtual ~SubscribeRequest();

private:
	SubscribeRequest(const std::list<SubscribeElement *>& subList);

	std::list<SubscribeElement *> _subscribeElements;

};

} // namespace
#endif /* SUBSCRIBEREQUEST_H_ */
