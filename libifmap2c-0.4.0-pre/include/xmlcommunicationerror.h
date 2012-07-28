/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2011, Arne Welzel, <arne.welzel@googlemail.com>
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

#ifndef XMLCOMMUNICATIONERROR_H_
#define XMLCOMMUNICATIONERROR_H_
#include <string>
#include <ostream>

namespace ifmap2c {

class XmlCommunicationError {

public:
	virtual const std::string& getMessage() const;
	virtual const std::string& getErrorType() const;
	
	virtual ~XmlCommunicationError();
	
	friend std::ostream& operator<<(std::ostream& output, const XmlCommunicationError& err);

protected:
	XmlCommunicationError(const std::string& type, const std::string& msg);

private:
	std::string _type;
	std::string _message;
	XmlCommunicationError();
};

class CommunicationError : public XmlCommunicationError {

public:
	CommunicationError(const std::string& msg) :
		XmlCommunicationError("CommunicationError", msg)
	{ }
};

} // namespace
#endif /* XMLCOMMUNICATIONERROR_H_ */
