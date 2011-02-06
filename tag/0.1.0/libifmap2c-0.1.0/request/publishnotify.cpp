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

#include "publishnotify.h"
#include "typedefs.h"
#include "tcgifmapbase.h"

namespace ifmap2c {

PublishNotify::PublishNotify(const XMLMLIST& mlist,
		Identifier *const i1,
		Identifier *const i2) :
		SubPublish(PUBLISH_NOTIFY_ELEMENT_NAME, i1, i2)
{
	BasicXmlMarshalable *metadatachild = new BasicXmlMarshalable(
			METADATA_LIST_ELEMENT_NAME, EMPTY_VALUE,
			NO_NSPAIR);

	CXMLMLISTIT it = mlist.begin();
	CXMLMLISTIT end = mlist.end();

	for (/* */; it != end; it++)
		metadatachild->addXmlChild(*it);

	addXmlChild(metadatachild);
}



PublishNotify *
PublishNotify::createPublishNotify(const XMLMLIST& metadataList,
		Identifier *const i1,
		Identifier *const i2)
{
	return new PublishNotify(metadataList, i1, i2);
}



PublishNotify *
PublishNotify::createPublishNotify(XmlMarshalable *const metadata,
		Identifier *const i1,
		Identifier *const i2)
{
	XMLMLIST list;
	list.push_back(metadata);
	return new PublishNotify(list, i1, i2);

}

} // namespace
