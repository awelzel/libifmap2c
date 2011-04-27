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

#ifndef LOWLEVELCURLCOMMUNICATION_H_
#define LOWLEVELCURLCOMMUNICATION_H_

#include "lowlevelcommunication.h"
#include "payload.h"
#include <string>


extern "C" {
	#include <curl/curl.h>
}

/* maximum len of a status message */
#define MAX_STATUS_LEN 128
#define HTTP_HDR "HTTP/1.1 "
#define HTTP_HDR_LEN strlen("HTTP/1.1 ")

namespace ifmap2c {

class LowLevelCurlCommunication : public LowLevelCommunication {

public:

	/*
	 * Destructor
	 */
	~LowLevelCurlCommunication();

	/*
	 * Implementation of abstract method
	 */
	Payload doRequest(const Payload &p);

	/*
	 * Basic Authentication
	 */
	static LowLevelCommunication *create(const std::string& url,
			const std::string& user,
			const std::string& pass,
			const std::string& capath);

	/*
	 * Certificate based authentication
	 */
	static LowLevelCommunication *create(const std::string& url,
			const std::string& mykey,
			const std::string& keypw,
			const std::string& mycert,
			const std::string& capath);


private:
	// handle for curl
	CURL *_handle;

	// used to exchange data between curl and this object
	Payload *_payloadExchange;

	// string list for curl :-/
	struct curl_slist *_slist;

	// errorBuffer for curl to store error messages in
	char *_errorBuffer;

	// place where the status message is put
	char *_statusBuffer;

	static bool _initialized;

	/*
	 * Constructor
	 */
	LowLevelCurlCommunication(CURL *const handle,
			Payload *const pexchanger,
			struct curl_slist *const slist,
			char *const errBuf,
			char *const statBuf);


	static void generalInitialization(CURL *const handle,
			Payload *const ex,
			struct curl_slist **slist,
			char **const errBuf,
			char **const statBuf,
			const std::string& url,
			const std::string& capath);

	static void curlInitialization();
};

} // namespace
#endif /* LOWLEVELCURLCOMMUNICATION_H_ */
