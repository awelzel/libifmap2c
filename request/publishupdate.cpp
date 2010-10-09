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

#include "publishupdate.h"
#include "typedefs.h"
#include "tcgifmapbase.h"

using namespace std;

namespace LIBRARY_NAMESPACE {

string PublishUpdate::lifeTimeNames[] = {
	"session",
	"forever"
};



PublishUpdate *
PublishUpdate::createPublishUpdate(const CLIST& mlist,
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
	CLIST metadataList;
	metadataList.push_back(metadata);
	return new PublishUpdate(metadataList, i1, lTime, i2);
}



PublishUpdate::~PublishUpdate()	{ }



PublishUpdate::PublishUpdate(const CLIST& mlist,
		Identifier *const i1, LifeTimeType lTime, Identifier *const i2) :
	  SubPublish(PUBLISH_UPDATE_ELEMENT_NAME, i1, i2)
{
	addXmlAttribute(STRP(PUBLISH_UPDATE_LIFET_ATTR_NAME, lifeTimeNames[lTime]));

	BasicXmlMarshalable *metadatachild = new BasicXmlMarshalable(
			METADATA_LIST_ELEMENT_NAME, EMPTY_VALUE,
			NO_NSPAIR);

	CCLISTIT it = mlist.begin();
	CCLISTIT end = mlist.end();

	for (/* */; it != end; it++)
		metadatachild->addXmlChild(*it);

	addXmlChild(metadatachild);
}

} // namespace
