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

#ifndef RESPONSECONVERTER_H_
#define RESPONSECONVERTER_H_

#include "xmlmarshalable.h"
#include "responses.h"
#include "errorresulterror.h"
#include "responseparseerror.h"

namespace ifmap2c {

class ResponseParser {
public:
	static NewSessionResult *createNewSessionResult(XmlMarshalable *const env);

	static void checkPublishReceived(XmlMarshalable *const env);

	static void checkSubscribeReceived(XmlMarshalable *const env);

	static void checkEndSessionResult(XmlMarshalable *const env);

	static void checkRenewSessionResult(XmlMarshalable *const env);

	static SearchResult *createSearchResult(XmlMarshalable *const env);

	static PollResult *createPollResult(XmlMarshalable *const env);

	static std::string extractSearchResultName(XmlMarshalable *const sr);

	static ResultItem *createResultItem(XmlMarshalable *xmlResultItem);

	static AccessRequest *extractAccessRequest(XmlMarshalable *const element);

	static IpAddress *extractIpAddress(XmlMarshalable *const element);

	static MacAddress *extractMacAddress(XmlMarshalable *const element);

	static Device *extractDevice(XmlMarshalable *const element);

	static Identity *extractIdentity(XmlMarshalable *const element);

	static XmlMarshalable *extractMetadataList(XmlMarshalable *const element);

	static SearchResult *extractSearchResult(XmlMarshalable *const element);

	static XmlMarshalable * locateUnderResponse(XmlMarshalable *const env,
			const std::pair<std::string, std::string>& eldesc);

	static XmlMarshalable *locateResponseElement(XmlMarshalable *const env);

	static XmlMarshalable *locateNewSessionResultElement(XmlMarshalable *const env);

	static XmlMarshalable *locateSearchResultElement(XmlMarshalable *const env);

	static XmlMarshalable *locatePollResultElement(XmlMarshalable *const env);

	static XmlMarshalable *locateEndSessionResultElement(XmlMarshalable *const env);

	static XmlMarshalable *locateSubscribeReceivedElement(XmlMarshalable *const env);

	static XmlMarshalable *locatePublishReceivedElement(XmlMarshalable *const env);

	static XmlMarshalable *locateRenewSessionResultElement(XmlMarshalable *const env);

	static bool compNameNs(XmlMarshalable *const elem,
			const std::string& name,
			const std::string& href);

	static bool isAttrWithName(const std::pair<std::string, std::string>& attr,
			const std::string& attrname);

	static XmlMarshalable *getChild(XmlMarshalable *marsh,
			const std::string& elname,
			const std::string& href);

	static void checkErrorResult(XmlMarshalable *const response);

	static void throwErrorResult(XmlMarshalable *const err);

	static void throwIfNull(XmlMarshalable *const m, const std::string& msg);

private:
	ResponseParser();
};

} // namespace
#endif /* RESPONSECONVERTER_H_ */
