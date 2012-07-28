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

#include "lowlevelcurlcommunication.h"
#include "xmlcommunicationerror.h"
#include "soap.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define	CURL_DEBUG_VALUE 0L
#define	CURL_VERIFY_HOST 0L
#define	CURL_VERIFY_PEER 1L

namespace ifmap2c {

bool LowLevelCurlCommunication::_initialized = false;


/*
 * local callback functions for curl
 */
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
static size_t read_data(void *buffer, size_t size, size_t nmemb, void *userp);
static size_t header_cb(void *buffer, size_t size, size_t nmemb, void *userp);



LowLevelCurlCommunication::LowLevelCurlCommunication(CURL * const h,
		Payload *const pexchanger, struct curl_slist *const sl,
		char *const errBuf, char *const statBuf) :
	_handle(h),
	_payloadExchange(pexchanger),
	_slist(sl),
	_errorBuffer(errBuf),
	_statusBuffer(statBuf)
{ }


LowLevelCurlCommunication::~LowLevelCurlCommunication()
{
	if (_handle)
		curl_easy_cleanup(_handle);
	_handle = NULL;

	if (_slist)
		curl_slist_free_all(_slist);
	_slist = NULL;

	if (_payloadExchange)
		delete _payloadExchange;
	_payloadExchange = NULL;

	if (_errorBuffer)
		delete[] _errorBuffer;
	_errorBuffer = NULL;

	if (_statusBuffer)
		delete[] _statusBuffer;
	_statusBuffer = NULL;
}

LowLevelCommunication *
LowLevelCurlCommunication::create(const string& url,
	const string& user, const string& pass, const string& capath)
{
	Payload *exchanger = new Payload(0, 0);
	CURL *handle = curl_easy_init();
	struct curl_slist *slist = NULL;
	char *errBuf = NULL;
	char *statBuf = NULL;

	generalInitialization(handle, exchanger, &slist, &errBuf,
			&statBuf, url, capath);

	// set-up basic auth stuff
	curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_easy_setopt(handle, CURLOPT_USERNAME, user.c_str());
	curl_easy_setopt(handle, CURLOPT_PASSWORD, pass.c_str());

	return new LowLevelCurlCommunication(handle, exchanger, slist,
			errBuf, statBuf);
}



LowLevelCommunication *
LowLevelCurlCommunication::create(const string& url, const string& mykey,
		const string& keypw, const string& mycert,
		const string& capath)
{
	Payload *exchanger = new Payload(0, 0);
	CURL *handle = curl_easy_init();
	struct curl_slist *slist = NULL;
	char *errBuf = NULL;
	char *statBuf = NULL;

	generalInitialization(handle, exchanger, &slist, &errBuf,
			&statBuf, url, capath);

	// set up cert auth stuff
	curl_easy_setopt(handle, CURLOPT_SSLKEY, mykey.c_str());
	curl_easy_setopt(handle, CURLOPT_SSLKEYPASSWD, keypw.c_str());
	curl_easy_setopt(handle, CURLOPT_SSLCERT, mycert.c_str());

	return new LowLevelCurlCommunication(handle, exchanger, slist,
			errBuf, statBuf);
}



void LowLevelCurlCommunication::generalInitialization(CURL *const handle,
		Payload *const ex, struct curl_slist **slist,
		char **const errBuf,
		char **const statBuf,
		const string& url,
		const string& capath)
{
	curlInitialization();

	// enable debugging
	curl_easy_setopt(handle, CURLOPT_VERBOSE, CURL_DEBUG_VALUE);

	// the url to connect to
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

	// SSL setup
	// drop possible standard certificate location
	curl_easy_setopt(handle, CURLOPT_CAINFO, NULL);
	// verify if the peer certificate is known
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, CURL_VERIFY_PEER);
	// verify if host name is valid
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, CURL_VERIFY_HOST);
	curl_easy_setopt(handle, CURLOPT_CAPATH, capath.c_str());

	curl_easy_setopt(handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);

	 // remove expect header FIXME: Check whether we can clean it
	 // after using, or do we have to wait, till the handle isn't used
	 // anymore???
	*slist = curl_slist_append(*slist, "Expect:");
	*slist = curl_slist_append(*slist, "Accept:");
	*slist = curl_slist_append(*slist, "Content-Type: " SOAP_CONTENT_TYPE);
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, *slist);

	// use POST
	curl_easy_setopt(handle, CURLOPT_POST, 1L);

	// callbacks
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &write_data);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, &read_data);
	curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, &header_cb);

	// payload object given through the last pointer of the callback functions
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, ex);
	curl_easy_setopt(handle, CURLOPT_READDATA, ex);

	// error buffer
	*errBuf = new char[CURL_ERROR_SIZE];
	*statBuf = new char[MAX_STATUS_LEN];
	
	// Initialize first byte to 0, otherwise we have compare
	// uninitialized data in doRequest()
	(*statBuf)[0] = 0;

	curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, *errBuf);
	curl_easy_setopt(handle, CURLOPT_WRITEHEADER, *statBuf);
}



void
LowLevelCurlCommunication::curlInitialization()
{
	if (!_initialized)
		curl_global_init(CURL_GLOBAL_ALL);
}



/*
 * FIXME: Could do some sanity checks ....
 */
Payload
LowLevelCurlCommunication::doRequest(const Payload &p)
{
	Payload ret(NULL, 0);
	// copy what the user gave us into our own memory
	_payloadExchange->memory = new char[p.length];
	_payloadExchange->length = p.length;
	memcpy(_payloadExchange->memory, p.memory, p.length);


	// set content length
	curl_easy_setopt(_handle, CURLOPT_POSTFIELDSIZE,
			(curl_off_t)_payloadExchange->length);

	/*
	 * Run exchange payloadExchange members will be different
	 * after completion. First read_data is called, which
	 * transfers the content of the Payload object to the server.
	 * The reply is fetched using write_data. After write_data
	 * the payloadExchange'er contains the reply.
	 */
	if (curl_easy_perform(_handle)) {
		delete[] _payloadExchange->memory;
		_payloadExchange->memory = NULL;
		_payloadExchange->length = 0;

		/* 
		 * if any other status than 200 was received
		 * _statusBuffer[0] is set
		 */
		if (_statusBuffer[0]) {
			_statusBuffer[MAX_STATUS_LEN - 1] = 0;
			throw CommunicationError(_statusBuffer);
		}
		_errorBuffer[CURL_ERROR_SIZE - 1] = 0;
		throw CommunicationError(string(_errorBuffer));
	} else {
		// give a memory reference back to the user
		ret = Payload(_payloadExchange->memory, _payloadExchange->length);
		_payloadExchange->memory = NULL;
		_payloadExchange->length = 0;
	}
	return ret;
}



/*
 * the write callback functions for curl :-(
 */
static size_t
write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	
	const size_t newsize = size * nmemb;
	Payload *exchanger = (Payload *)userp;
	char *memptr= exchanger->memory;
	size_t oldsize = exchanger->length;

	// we may be called twice by libcurl, make sure
	// we increase our buffer and copy the old content
	// and so on...
	exchanger->memory = new char[oldsize + newsize];
	exchanger->length = oldsize + newsize;
	// this is a nop if we are called the first time
	memcpy(exchanger->memory, memptr, oldsize);
	delete[] memptr;
	memptr = exchanger->memory + oldsize;

	memcpy(memptr, buffer, newsize);

	if (CURL_DEBUG_VALUE) {
		fprintf(stderr, "write_data: %d bytes received\n",
			exchanger->length);

		for (int i = 0; i < exchanger->length; i++) {
			fprintf(stderr, "%c", exchanger->memory[i]);
		}
		fprintf(stderr, "\n");
	}
	return newsize;
}



/*
 * the read callback functions for curl :-(
 */
static size_t
read_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	Payload *exchanger = (Payload *)userp;
	int curlsize = (size * nmemb);
	int exchgleft = (exchanger->length - exchanger->curPtr);
	int curPtr = exchanger->curPtr;
	int cpybytes = (curlsize < exchgleft) ?
		curlsize : exchgleft;

	// finished transferring data  to libcurl?
	// if yes, free everything...
	if (cpybytes == 0) {
		if (CURL_DEBUG_VALUE) {
			fprintf(stderr, "read_data: complete\n");
		}
		exchanger->length = 0;
		exchanger->curPtr = 0;
		/* free the data which was just sent */
		delete[] exchanger->memory;
		exchanger->memory = NULL;
	} else {
		memcpy(buffer, exchanger->memory + curPtr, cpybytes);
		exchanger->curPtr += cpybytes;
		if (CURL_DEBUG_VALUE) {
			fprintf(stderr, "read_data: %d bytes to be transferred\n",
				cpybytes);

			for (int i = 0; i < cpybytes; i++) {
				fprintf(stderr, "%c", ((char *)buffer)[i]);
			}
			fprintf(stderr, "\n");
		}
	}
	return cpybytes;
}

static size_t
header_cb(void *buffer, size_t size, size_t nmemb, void *userp)
{
	char *statusBuffer = (char*)userp;
	char *newLine = NULL;
	
	int cpyLen = (size * nmemb) < MAX_STATUS_LEN ?
		(size * nmemb) : MAX_STATUS_LEN;
	int cmpLen = (size * nmemb) < HTTP_HDR_LEN ?
		(size * nmemb) : HTTP_HDR_LEN;

	/* 
	 * If after curl_easy_perform() _statusBuffer[0] != 0,
	 * we need to throw an communication error with
	 * _statusBuffer as message...
	 */
	statusBuffer[0] = 0;

	/* is this the status header ? */
	if (!strncmp((char*)buffer, HTTP_HDR, cmpLen)) {
		strncpy(statusBuffer, (char *)buffer, cpyLen);
		statusBuffer[cpyLen] = 0;

		/* we only expect status 200 messages */
		if (strncmp(statusBuffer + HTTP_HDR_LEN, "200", 3)) {
			
			/* truncate traling newline */
			newLine = strchr(statusBuffer, '\n');
			if (newLine)
				*newLine = 0;

			/* signal error */
			return 0;
		}
	}
	return size * nmemb;
}
} // namespace
