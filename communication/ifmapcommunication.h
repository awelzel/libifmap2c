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

#ifndef IFMAPCOMMUNICATION_H_
#define IFMAPCOMMUNICATION_H_

#include "requests.h"
#include "responses.h"
#include "tcgifmapbase.h"
#include "xmlmarshaller.h"
#include "xmlunmarshaller.h"
#include "lowlevelcommunication.h"
#include "responseparser.h"
#include "responseparseerror.h"
#include "communicationerror.h"

namespace LIBRARY_NAMESPACE {


class IfmapCommunication {

public:
	virtual const std::string& getSessionId(void) const = 0;
	virtual const std::string& getPublisherId(void) const = 0;
	virtual const std::string& getMaxPollResultSize(void) const = 0;

	virtual ~IfmapCommunication();

protected:
	IfmapCommunication(const std::string& url,
			const std::string& user,
			const std::string& pass,
			const std::string& capath);


	IfmapCommunication(const std::string& url,
			const std::string& mykey,
			const std::string& mykeypw,
			const std::string& mycert,
			const std::string& capath);

	XmlMarshalable *processMessage(XmlMarshalable *const msg);

	bool _basicAuth;
	std::string _url;
	std::string _userName;
	std::string _keyFile;
	std::string _certFile;
	std::string _password;
	std::string _caPath;

private:
	LowLevelCommunication *_lowLevelCommunication;
	XmlMarshaller *_xmlMarshaller;
	XmlUnmarshaller *_xmlUnmarshaller;

	bool containsSessionId(XmlMarshalable *req);

	XmlMarshalable *buildEnvelope();
};

} // namespace

#endif /* IFMAPCOMMUNICATION_H_ */
