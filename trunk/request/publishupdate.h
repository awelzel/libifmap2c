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

#ifndef UPDATEREQUEST_H_
#define UPDATEREQUEST_H_

#include "subpublish.h"
#include <list>

namespace LIBRARY_NAMESPACE {

enum LifeTimeType {
	session,
	forever
};

class PublishUpdate : public SubPublish
{
public:

	static PublishUpdate*
	createPublishUpdate(const std::list<XmlMarshalable *>& metadataList,
			Identifier *const i1,
			LifeTimeType lTime = session,
			Identifier *const i2 = NULL);

	static PublishUpdate*
	createPublishUpdate(XmlMarshalable *const metadata,
			Identifier *const i1,
			LifeTimeType lTime = session,
			Identifier *const i2 = NULL);

	virtual ~PublishUpdate();

private:
	PublishUpdate(const std::list<XmlMarshalable *>& mlist,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2);


	static std::string lifeTimeNames[];

};

} // namespace
#endif /* UPDATEREQUEST_H_ */
