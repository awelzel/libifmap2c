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

#include "ifmapchannel.h"
#include "xml/libxml2binding.h"
#include "communication/lowlevelcurlcommunication.h"
#include "ifmaprequest.h"

using namespace std;

namespace ifmap2c {

IfmapChannel::~IfmapChannel()
{
	delete _xmlCommunication;
}

IfmapChannel::IfmapChannel(const string& url,
		const string& user,
		const string& pass,
		const string& capath)
{ 
	LowLevelCommunication *llc = 
		LowLevelCurlCommunication::create(url, user, pass, capath);

	XmlMarshaller *xmlMarsh = new LibXml2Marshaller();
	XmlUnmarshaller *xmlUnmarsh = new LibXml2Unmarshaller();
	RequestHandlerDispatch *reqDisp = new IfmapRequestHandlerDispatch();

	_xmlCommunication = new XmlCommunication(llc, xmlMarsh,
			xmlUnmarsh, reqDisp);
}

IfmapChannel::IfmapChannel(const string& url,
		const string& mykey,
		const string& mykeypw,
		const string& mycert,
		const string& capath)
{
	LowLevelCommunication *llc = 
		LowLevelCurlCommunication::create(url, mykey, mykeypw,
				mycert, capath);

	XmlMarshaller *xmlMarsh = new LibXml2Marshaller();
	XmlUnmarshaller *xmlUnmarsh = new LibXml2Unmarshaller();
	RequestHandlerDispatch *reqDisp = new IfmapRequestHandlerDispatch();

	_xmlCommunication = new XmlCommunication(llc, xmlMarsh,
			xmlUnmarsh, reqDisp);
}

} // namespace
