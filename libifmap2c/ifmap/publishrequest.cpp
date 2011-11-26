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
#include "typedefs.h"

using namespace std;

namespace ifmap2c {

PublishRequest *
PublishRequest::createPublishRequest(const PELIST& rList)
{
	return new PublishRequest(rList);
}

PublishRequest *
PublishRequest::createPublishRequest(PublishElement *const subPublishRequest)
{
	PELIST rList;
	rList.push_back(subPublishRequest);
	return new PublishRequest(rList);
}

PublishRequest::PublishRequest(const PELIST& rList) :
	_publishElements(rList)
{ }

PublishRequest::~PublishRequest() {
	CPELISTIT it, end;
	it = _publishElements.begin();
	end = _publishElements.end();

	for (/* */; it != end; it++)
		delete *it;
}

const PELIST&
PublishRequest::getPublishElements(void) const
{
	return _publishElements;
}

string PublishUpdate::lifeTimeNames[] = {
	"session",
	"forever"
};

PublishUpdate *
PublishUpdate::createPublishUpdate(const XMLMLIST& mlist,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2)
{
	return new PublishUpdate(mlist, i1, lTime, i2);
}

PublishUpdate *
PublishUpdate::createPublishUpdate(XmlMarshalable *const metadata,
		Identifier *const i1,
		LifeTimeType lTime,
		Identifier *const i2)
{
	XMLMLIST metadataList;
	metadataList.push_back(metadata);
	return new PublishUpdate(metadataList, i1, lTime, i2);
}

PublishUpdate::~PublishUpdate()	{ }

PublishUpdate::PublishUpdate(const XMLMLIST& mlist,
	Identifier *const i1,
	LifeTimeType lTime,
	Identifier *const i2) :
	IdentifierMetadataHolder(i1, i2, mlist),
	_lifeTime(lTime)
{ }

LifeTimeType PublishUpdate::getLifeTime(void) const
{
	return _lifeTime;
}

const string PublishUpdate::getLifeTimeString(void) const
{
	return lifeTimeNames[getLifeTime()];
}

PublishDelete::PublishDelete(const char *const filter,
		Identifier *const i1, Identifier *const i2) :
		PublishElement(), IdentifierHolder(i1, i2),
		_filter(filter)
{ }

const char *PublishDelete::getFilter(void) const
{
	return _filter;
}

PublishDelete *
PublishDelete::createPublishDelete(const char *const filter,
		Identifier *const i1, Identifier *const i2)
{
	return new PublishDelete(filter, i1, i2);
}

PublishNotify::PublishNotify(const XMLMLIST& mlist,
		Identifier *const i1, Identifier *const i2) :
		IdentifierMetadataHolder(i1, i2, mlist)
{ }

PublishNotify *
PublishNotify::createPublishNotify(const XMLMLIST& metadataList,
		Identifier *const i1, Identifier *const i2)
{
	return new PublishNotify(metadataList, i1, i2);
}

PublishNotify *
PublishNotify::createPublishNotify(XmlMarshalable *const metadata,
		Identifier *const i1, Identifier *const i2)
{
	XMLMLIST list;
	list.push_back(metadata);
	return new PublishNotify(list, i1, i2);

}

} // namespace
