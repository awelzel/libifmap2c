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

#include "publishrequest.h"
//#include "typedefs.h"
//#include "tcgifmapbase.h"

using namespace std;

namespace ifmap2c {

PublishRequest *
PublishRequest::createPublishRequest(const std::list<PublishElement *>& rList)
{
	return new PublishRequest(rList);
}

PublishRequest *
PublishRequest::createPublishRequest(PublishElement *const subPublishRequest)
{
	list<PublishElement *> rList;
	rList.push_back(subPublishRequest);
	return new PublishRequest(rList);
}

PublishRequest::PublishRequest(const list<PublishElement *>& rList) :
	_publishElement(rList)
{
	/*
	BasicXmlMarshalable(PUBLISH_ELEMENT_NAME, EMPTY_VALUE,
			IFMAP_OPERATION_NSPAIR)
	list<PublishElement *>::const_iterator it = rList.begin();
	list<PublishElement *>::const_iterator end = rList.end();

	for (// //; it != end; it++)
		addXmlChild(*it);
	*/
}

PublishRequest::~PublishRequest() { }

const list<PublishElement *>
PublishRequest::getPublishElements(void) const
{
	return _publishElement;
}

} // namespace
