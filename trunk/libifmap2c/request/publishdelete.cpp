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

#include "publishdelete.h"
#include "typedefs.h"
#include "tcgifmapbase.h"

namespace ifmap2c {

PublishDelete::PublishDelete(const char *const filter,
		Identifier *const i1,
		Identifier *const i2) :
		PublishElement(PUBLISH_DELETE_ELEMENT_NAME, i1, i2)
{
	if (filter)
		addXmlAttribute(STRP(PUBLISH_DELETE_FILTER_ATTR_NAME, filter));
}



PublishDelete *
PublishDelete::createPublishDelete(const char *const filter,
		Identifier *const i1,
		Identifier *const i2)
{
	return new PublishDelete(filter, i1, i2);
}

} // namespace