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

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_
#include <string>
#include <list>

/* 
 * Forward declarations for the typedef'ed classes.
 *
 * If we include the headers directly, we need to recompile nearly
 * everything because typedefs.h is include nearly everywhere.
 */
class ErrorResult;
class ResultItem;
class SearchResult;
class XmlMarshalable;

extern "C" {
	#include <libxml/tree.h>
}

namespace ifmap2c {

// STRP = string pair
typedef std::pair<std::string, std::string> STRP;
typedef std::list<STRP> STRPLIST;
typedef STRPLIST::iterator STRPLISTIT;
typedef STRPLIST::const_iterator CSTRPLISTIT;

// XMLM == XmlMarshalable
typedef std::list<XmlMarshalable *> XMLMLIST;
typedef const XMLMLIST CXMLMLIST;
typedef XMLMLIST::iterator XMLMLISTIT;
typedef XMLMLIST::const_iterator CXMLMLISTIT;

// SR = SearchResult
typedef std::list<SearchResult *> SRLIST;
typedef const SRLIST CSRLIST;
typedef SRLIST::iterator SRLISTIT;
typedef SRLIST::const_iterator CSRLISTIT;

// ER = ErrorResult
typedef std::list<ErrorResult *> ERLIST;
typedef const ERLIST CERLIST;
typedef ERLIST::iterator ERLISTIT;
typedef ERLIST::const_iterator CERLISTIT;

// RI = ResultItem
typedef std::list<ResultItem *> RILIST;
typedef const RILIST CRILIST;
typedef RILIST::iterator RILISTIT;
typedef RILIST::const_iterator CRILISTIT;

typedef std::pair<std::string, xmlNsPtr> NSDECL;
typedef std::list<NSDECL> NSDECLLIST;
typedef NSDECLLIST::reverse_iterator NSDECLLISTIT;

} // namespace
#endif /* TYPEDEFS_H_ */
