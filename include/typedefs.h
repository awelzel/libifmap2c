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

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include "searchresult.h"
#include "xmlmarshalable.h"
#include "resultitem.h"

#include <string>
#include <list>

extern "C" {
	#include <libxml/tree.h>
}

namespace ifmap2c {

typedef std::pair<std::string, std::string> STRP;
typedef std::list<STRP> STRPLIST;
typedef std::list<STRP>::iterator STRPLISTIT;
typedef std::list<STRP>::const_iterator CSTRPLISTIT;
typedef std::list<XmlMarshalable *> CLIST;
typedef const std::list<XmlMarshalable *> CCLIST;
typedef std::list<XmlMarshalable *>::iterator CLISTIT;
typedef std::list<XmlMarshalable *>::const_iterator CCLISTIT;

typedef std::list<SearchResult *> SRLIST;
typedef const SRLIST CSRLIST;
typedef SRLIST::iterator SRLISTIT;
typedef SRLIST::const_iterator CSRLISTIT;

typedef std::list<ResultItem *> RILIST;
typedef const RILIST CRILIST;
typedef RILIST::iterator RILISTIT;
typedef RILIST::const_iterator CRILISTIT;

typedef std::pair<std::string, xmlNsPtr> NSDECL;
typedef std::list<NSDECL> NSDECLLIST;
typedef NSDECLLIST::reverse_iterator NSDECLLISTIT;

} // namespace
#endif /* TYPEDEFS_H_ */
