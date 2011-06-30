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

#include "errorresulterror.h"

using namespace std;

namespace ifmap2c {

const string ErrorResultError::errorCodeStrings[] = {
        "AccessDenied",
        "Failure",
        "InvalidIdentifier",
        "InvalidIdentifierType",
        "IdentifierTooLong",
        "InvalidMetadata",
        "InvalidSchemaVersion",
        "InvalidSessionID",
        "MetadataTooLong",
        "SearchResultsTooBig",
        "PollResultsTooBig",
        "SystemError",
        "Unknown"
};

ErrorResultError::ErrorResultError(ErrorCode errCode,
		const std::string& errStr,
		const std::string& name) :
	_errorCode(errCode), _errorString(errStr), _name(name)
{ }


ErrorResultError::~ErrorResultError()
{ }


ErrorCode
ErrorResultError::getErrorCode() const
{
	return _errorCode;
}

const std::string&
ErrorResultError::getErrorCodeString() const
{
	return errorCodeStrings[_errorCode];
}

const std::string&
ErrorResultError::getErrorString() const
{
	return _errorString;
}

ostream& operator<<(ostream& output, const ErrorResultError& err)
{
	output << err.getErrorCodeString() << ": ";
	output << err.getErrorString();
	return output;
}

} // namespace
