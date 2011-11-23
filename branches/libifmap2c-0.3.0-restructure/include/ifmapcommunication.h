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

#ifndef IFMAPCOMMUNICATION_H_
#define IFMAPCOMMUNICATION_H_
#include <string>

#include "communicationerror.h"
#include "ifmaperror.h"
#include "lowlevelcommunication.h"
#include "requests.h"
#include "responseparseerror.h"
#include "responseparser.h"
#include "responses.h"
#include "request.h"
#include "result.h"
#include "tcgifmapbase.h"
#include "xmlmarshaller.h"
#include "xmlunmarshaller.h"

namespace ifmap2c {


class IfmapCommunication {

public:
	/**
	 * @return the current session-id or an empty string
	 * if none is set.
	 */
	virtual const std::string& getSessionId(void) const = 0;

	/**
	 * @return the current publisher-id or an empty string
	 * if none is set.
	 */
	virtual const std::string& getPublisherId(void) const = 0;

	/**
	 * @eturn the current max-poll-result-size, or
	 * NO_MAX_POLL_RES_SIZE if no max-poll-result size was
	 * specified.
	 */
	virtual int getMaxPollResultSize(void) const = 0;

	virtual ~IfmapCommunication();

	Result *genericRequest(Request *const req,
			const std::string& sId = "");
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


	/**
	 * Set the a session-id attribute to the request.
	 *
	 * If the is already a session-id attribute set, the old value is
	 * removed and replaced by the new one.
	 *
	 * @param req the request for which the session-id attribute is to be set
	 * @param sessionId this value will be set as session-id attribute
	 */
	void setSessionId(XmlMarshalable *const req,
			const std::string& sessionId);

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
	
	/**
	 *
	 */
	XmlMarshalable *processMessage(XmlMarshalable *const msg);
};

} // namespace

#endif /* IFMAPCOMMUNICATION_H_ */
