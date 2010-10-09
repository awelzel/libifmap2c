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

#ifndef LOWLEVELCOMMUNICATION_H_
#define LOWLEVELCOMMUNICATION_H_
#include "payload.h"

namespace LIBRARY_NAMESPACE {

class LowLevelCommunication {

	public:
		virtual ~LowLevelCommunication() { };
		/* 
		 * Send the memory area in p to the other peer,
		 * return the response in another Payload object
		 *
		 * The caller is responsible to free the memory
		 * area in the returend Payload object and is of
		 * course responsible to free the memory in th
		 * given Payload object
		 */
		virtual Payload doRequest(const Payload &p) = 0;

};

} // namespace
#endif /* LOWLEVELCOMMUNICATION_H_ */
