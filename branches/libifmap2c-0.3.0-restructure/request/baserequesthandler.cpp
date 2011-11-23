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

#include "request/baserequesthandler.h"
#include "basicxmlmarshalable.h"
#include "typedefs.h"
#include <string>


using namespace std;
namespace ifmap2c {
IFMAP2C_RH_TOXML_DEF(NewSessionRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(NewSessionRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(EndSessionRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(EndSessionRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(RenewSessionRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(RenewSessionRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(PurgePublisherRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(PurgePublisherRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(PublishRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(PublishRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(SearchRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(SearchRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(SubscribeRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(SubscribeRequest, xml)
{
	return (Result*)xml;
}

IFMAP2C_RH_TOXML_DEF(PollRequest, req)
{
	return (XmlMarshalable *)req;
}

IFMAP2C_RH_FROMXML_DEF(PollRequest, xml)
{
	return (Result*)xml;
}

}
