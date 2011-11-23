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

#ifndef BASEREQUESTHANDLER_H_
#define BASEREQUESTHANDLER_H_
#include "request.h"
#include "requests.h"

namespace ifmap2c {

	IFMAP2C_RH_HEADER(NewSessionRequest);

	IFMAP2C_RH_HEADER(EndSessionRequest);

	IFMAP2C_RH_HEADER(RenewSessionRequest);
	
	IFMAP2C_RH_HEADER(PurgePublisherRequest);

	IFMAP2C_RH_HEADER(PublishRequest);

	IFMAP2C_RH_HEADER(SearchRequest);
	
	IFMAP2C_RH_HEADER(SubscribeRequest);
	
	IFMAP2C_RH_HEADER(PollRequest);

} // namespace

#endif /* BASEREQUESTHANDLER_H_ */
