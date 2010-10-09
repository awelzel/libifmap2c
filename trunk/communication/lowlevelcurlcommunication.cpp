/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2010, A. Welzel, <arne.welzel@gmail.com>
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
#include "communicationerror.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace std;

#define	CURL_DEBUG_VALUE 0
#define	CURL_VERIFY_HOST 0

namespace LIBRARY_NAMESPACE {

bool LowLevelCurlCommunication::initialized = false;


/*
 * local callback functions for curl
 */
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
static size_t read_data(void *buffer, size_t size, size_t nmemb, void *userp);



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

	if (errorBuffer)
		delete[] errorBuffer;
	errorBuffer = NULL;
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
	curl_easy_setopt(_handle, CURLOPT_INFILESIZE,
			(curl_off_t)_payloadExchange->length);

	// Run exchange payloadExchange members will be different
	// after completion. First read_data is called, which
	// transfers the content of the Payload object to the server.
	// The reply is fetched using write_data. After write_data
	// the payloadExchange'er contains the reply.
	if (curl_easy_perform(_handle)) {
		delete[] _payloadExchange->memory;
		_payloadExchange->memory = NULL;
		_payloadExchange->length = 0;
		throw CommunicationError(string(errorBuffer));
	} else {
		// give a memory reference back to the user
		ret = Payload(_payloadExchange->memory, _payloadExchange->length);
		_payloadExchange->memory = NULL;
		_payloadExchange->length = 0;
	}
	return ret;
}



LowLevelCommunication *
LowLevelCurlCommunication::create(const string& url,
	const string& user, const string& pass, const string& capath)
{
	Payload *exchanger = new Payload(0, 0);
	CURL *handle = curl_easy_init();
	struct curl_slist *slist = NULL;
	char *errBuf = NULL;

	generalInitialization(handle, exchanger, &slist, &errBuf, url, capath);

	// set-up basic auth stuff
	curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_easy_setopt(handle, CURLOPT_USERNAME, user.c_str());
	curl_easy_setopt(handle, CURLOPT_PASSWORD, pass.c_str());

	return new LowLevelCurlCommunication(handle, exchanger, slist, errBuf);
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

	generalInitialization(handle, exchanger, &slist, &errBuf, url, capath);

	// set up cert auth stuff
	curl_easy_setopt(handle, CURLOPT_SSLKEY, mykey.c_str());
	curl_easy_setopt(handle, CURLOPT_SSLKEYPASSWD, keypw.c_str());
	curl_easy_setopt(handle, CURLOPT_SSLCERT, mycert.c_str());

	return new LowLevelCurlCommunication(handle, exchanger, slist, errBuf);
}



LowLevelCurlCommunication::LowLevelCurlCommunication(CURL * const h,
		Payload *const pexchanger, struct curl_slist *const sl,
		char * const errBuf) :
	_handle(h),
	_payloadExchange(pexchanger),
	_slist(sl),
	errorBuffer(errBuf)
{ }



void LowLevelCurlCommunication::generalInitialization(CURL *const handle,
		Payload *const ex, struct curl_slist **slist,
		char **const errBuf,
		const string& url,
		const string& capath)
{
	curlInitialization();

	// enable debugging
	curl_easy_setopt(handle, CURLOPT_VERBOSE, CURL_DEBUG_VALUE);

	// the url to connect to
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

	// verify the peer, use the servercert file
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(handle, CURLOPT_CAPATH, capath.c_str());

	// verify the hostname
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, CURL_VERIFY_HOST);

	 // remove expect header FIXME: Check whether we can clean it
	 // after using, or do we have to wait, till the handle isn't used
	 // anymore???
	*slist = curl_slist_append(*slist, "Expect:");
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, *slist);

	// callbacks
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &write_data);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, &read_data);
	curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);

	// payload object given through the last pointer of the callback functions
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, ex);
	curl_easy_setopt(handle, CURLOPT_READDATA, ex);

	// error buffer
	*errBuf = new char[CURL_ERROR_SIZE];
	curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, *errBuf);
}



void
LowLevelCurlCommunication::curlInitialization()
{
	if (!initialized)
		curl_global_init(CURL_GLOBAL_ALL);
}



/*
 * the write callback functions for curl :-(
 */
static size_t
write_data(void *buffer, size_t size, size_t nmemb,
		void *userp)
{
	const size_t theSize = size * nmemb;
	Payload *exchanger = (Payload *)userp;

	exchanger->memory = new char[theSize];
	exchanger->length = theSize;

	memcpy(exchanger->memory, buffer, exchanger->length);

	if (CURL_DEBUG_VALUE) {
		fprintf(stderr, "write_data: %d bytes received\n",
			exchanger->length);

		for (int i = 0; i < exchanger->length; i++) {
			fprintf(stderr, "%c", exchanger->memory[i]);
		}
		fprintf(stderr, "\n");
	}
	return theSize;
}



/*
 * the read callback functions for curl :-(
 *
 * FIXME: Allow for big data transfers, e.g. calling
 * read_data multiple times?
 */
static size_t
read_data(void *buffer, size_t size, size_t nmemb,
		void *userp)
{
	Payload *exchanger = (Payload *)userp;
	size_t tmp;

	/* finish transfer ? */
	if (exchanger->length == 0)
		return 0;

	if ((unsigned int)exchanger->length > (size * nmemb)) {
		fprintf(stderr, "FAIL FAIL FAIL :-(\n");
		exit(1);
	}

	if (CURL_DEBUG_VALUE) {
		fprintf(stderr, "read_data: %d bytes to be sent\n",
			exchanger->length);

		for (int i = 0; i < exchanger->length; i++) {
			fprintf(stderr, "%c", exchanger->memory[i]);
		}
		fprintf(stderr, "\n");
	}

	/* copy everything in the exchanger to libcurl */
	memcpy(buffer, exchanger->memory, exchanger->length);
	tmp = exchanger->length;

	exchanger->length = 0;
	/* free the data which was just sent */
	delete[] exchanger->memory;
	exchanger->memory = NULL;

	return tmp;
}

} // namespace
