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

#include "macaddress.h"

#include "tcgifmapbase.h"
#include "typedefs.h"

using namespace std;

namespace LIBRARY_NAMESPACE {

MacAddress::MacAddress(const string& val, const string& ad) :
	IdentifierWithAD(MACADDR_ELEMENT_NAME, EMPTY_VALUE, ad), _value(val)
{
	addXmlAttribute(STRP(MACADDR_VALUE_ATTR_NAME, val));
}

MacAddress *MacAddress::createMacAddress(const string& val, const string& ad)
{
	return new MacAddress(val, ad);
}


const string&
MacAddress::getValue() const
{
	return _value;
}

} // namespace
