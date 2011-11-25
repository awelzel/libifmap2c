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

#ifndef IFMAPCHANNEL_H_
#define IFMAPCHANNEL_H_

#include "xmlcommunication.h"

#include <string>

namespace ifmap2c {

class IfmapChannel {

public:

	virtual const std::string& getSessionId(void) const;

	virtual const std::string& getPublisherId(void) const;

	virtual int getMaxPollResultSize(void) const;
	
	virtual ~IfmapChannel();

protected:

	IfmapChannel(const std::string& url,
		const std::string& user,
		const std::string& pass,
		const std::string& capath);

	IfmapChannel(const std::string& url,
		const std::string& mykey,
		const std::string& mykeypw,
		const std::string& mycert,
		const std::string& capath);

	XmlCommunication *_xmlCommunication;

	bool _basicAuth;
	std::string _url;
	std::string _userName;
	std::string _keyFile;
	std::string _certFile;
	std::string _password;
	std::string _caPath;
};

} // namespace
#endif /* IFMAPCHANNEL_H_ */
