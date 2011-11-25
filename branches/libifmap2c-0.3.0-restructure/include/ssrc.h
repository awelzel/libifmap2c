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

#ifndef SSRC_H_
#define SSRC_H_

#include "ifmapchannel.h"
#include "requests.h"

#include <string>

namespace ifmap2c {

// forward declaration for ARC, ARC depends on SSRC as well...
class ARC;

/**
 * SSRC encapsulates the Synchronous Send Receive Channel functionality
 * of IF-MAP.
 */
class SSRC : public IfmapChannel {

public:

	/**
	 * Create a SSRC to a MAPS using basic authentication.
	 *
	 * @param url the url to the MAPS, e.g https://mymaps.orga.org:8443
	 * @param user the username to be used for basic authentication
	 * @param pass the password to be used for basic authentication
	 * @param capath the path to the directory which contains the certificate
	 * 		 of the MAPS and is prepared with the c_rehash command
	 * 		 of OpenSSL
	 *
	 * @return pointer to a SSRC object which can be used to execute
	 * 	   SSRC operations and to allocate an ARC.
	 */
	static SSRC *createSSRC(const std::string& url,
			const std::string& user,
			const std::string& pass,
			const std::string& capath);

	/**
	 * Create a SSRC to a MAPS using certificate based authentication.
	 *
	 * @param url the url to the MAPS, e.g https://mymaps.orga.org:8443
	 * @param mykey path to the keyfile in PEM format
	 * @param mykeypass cleartext password for the keyfile
	 * @param mycert the certificate corresponding to the key
	 * @param capath the path to the directory which contains the certificate
	 * 		 of the MAPS and is prepared with the c_rehash command
	 * 		 of OpenSSL
	 *
	 * @return pointer to a SSRC object which can be used to execute
	 * 	   SSRC operations and to allocate an ARC.
	 */
	static SSRC* createSSRC(const std::string& url,
			const std::string& mykey,
			const std::string& mykeypw,
			const std::string& mycert,
			const std::string& capath);

	/**
	 * Deconstructor frees the used resources and closes the TCP connection
	 * to the server (CHECK).
	 */
	virtual ~SSRC();


	/**
	 * Allocate an ARC for this SSRC.
	 *
	 * The returned object can be used to do the poll operation.
	 * If the SSRC object, corresponding to a ARC, is deleted, the
	 * usage of the ARC is disallowed (The ARC contains a reference to
	 * it's SSRC).
	 *
	 * @return pointer to a ARC object which can be used to execute poll
	 *
	 */
	ARC *getARC(void);

	/**
	 * Do a newSession operation.
	 *
	 * This method should be called before any other method is called.
	 * Currently there are no state checks involved. After this method
	 * has successfully returned, getSessionId() and getPublisherId()
	 * will return the IDs allocated by the MAPS.
	 *
	 * @param maxPollResSize optional value for the max-poll-result-size
	 * 			 attribute
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	void newSession(const int maxPollResSize = NO_MAX_POLL_RES_SIZE);

	/**
	 * Do a endSession operation.
	 *
	 * This method send a endSession request to the MAPS. After calling
	 * this method only a call to newSession is allowed. Currently there
	 * are no state checks involved.
	 * The TCP connection is not necessarily closed after a call to
	 * this method.
	 *
	 * @param sessionId if this value is set to a string with length > 0,
	 *		    this value is used as the session-id, else the
	 *		    value obtained by getSessionId() will be used.
	 *		    (Only use it if you know what you are doing.)
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	void endSession(const std::string& sessionId = "");

	/**
	 * Do a publish operation.
	 *
	 * @param pr request to be send to the server containing update, delete
	 * 	     or notify. Should be constructed using
	 * 	     Requests::createPublishReq().
	 * @param sessionId if this value is set to a string with length > 0,
	 *		    this value is used as the session-id, else the
	 *		    value obtained by getSessionId() will be used.
	 *		    (Only use it if you know what you are doing.)
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	void publish(PublishRequest *const pr,
			const std::string& sessionId = "");

	/**
	 * Do a search operation.
	 *
	 * @param sr request to be send to the server. Should be constructed
	 * 	     using Requests::createSearchReq().
	 * @param sessionId if this value is set to a string with length > 0,
	 *		    this value is used as the session-id, else the
	 *		    value obtained by getSessionId() will be used.
	 *		    (Only use it if you know what you are doing.)
	 *
	 * @return pointer to a SearchResult object which contains the answer
	 *	   of the MAPS. The caller is responsible to delete the memory.
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	SearchResult *search(SearchRequest *const sr,
			const std::string& sessionId = "");

	/**
	 * Do a subscribe operation.
	 *
	 * @param sr request to be send to the server containing SubscribeUpdate
	 *	     or SubscribeDelete objects. Should be constructed using
	 *	     Requests::createSubscribeReq().
	 * @param sessionId if this value is set to a string with length > 0,
	 *		    this value is used as the session-id, else the
	 *		    value obtained by getSessionId() will be used.
	 *		    (Only use it if you know what you are doing.)
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	void subscribe(SubscribeRequest *const sr,
			const std::string& sessionId = "");

	/**
	 * Run a renewSession operation.
	 *
	 * @param sessionId if this value is set to a string with length > 0,
	 *		    this value is used as the session-id, else the
	 *		    value obtained by getSessionId() will be used.
	 *		    (Only use it if you know what you are doing.)
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	void renewSession(const std::string& sessionId = "");


	/**
	 * Run a purgePublisher operation.
	 *
	 * @param publisherId if this value is set to a string with length > 0,
	 * 		      this value is used as the publisher-id, else
	 *		      the value obtained by getPublisherId() will be
	 *		      used.
	 *		      (Only use it if you know what you are doing.)
	 * @param sessionId if this value is set to a string with length > 0,
	 *		    this value is used as the session-id, else the
	 *		    value obtained by getSessionId() will be used.
	 *		    (Only use it if you know what you are doing.)
	 *
	 * \throw IfmapError
	 * \throw ErrorResult
	 */
	void purgePublisher(const std::string& publisherId = "",
			const std::string& sessionId = "");

	const std::string& getSessionId(void) const;

	const std::string& getPublisherId(void) const;

	int getMaxPollResultSize(void) const;


private:
	SSRC(const std::string& url,
			const std::string& user,
			const std::string& pass,
			const std::string& capath);

	SSRC(const std::string& url,
			const std::string& mykey,
			const std::string& mykeypw,
			const std::string& mycert,
			const std::string& capath);

	std::string _currentSessionId;
	std::string _currentPublisherId;
	int _currentMaxPollResSize;
};

} // namespace
#endif /* SSRC_H_ */
