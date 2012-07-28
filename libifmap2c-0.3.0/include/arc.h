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

#ifndef ARC_H_
#define ARC_H_

#include "ifmapcommunication.h"
#include "ssrc.h"

namespace ifmap2c {

class ARC : public IfmapCommunication {

public:


	/**
	 * Start a poll request. This method may throw a EndSessionResult
	 * object if the underlying IF-MAP session is closed while the poll
	 * was pending.
	 *
	 * @return a pointer to a PollResult, the caller is responsible
	 *         for freeing the memory.
	 * @throws IfmapException if communication, marshalling or
	 *         unmarshalling fail.
	 * @throws ErrorResult if the server returns an errorResult.
	 * @throws EndSessionResult if the session was closed while a poll
	 *         was pending.
	 */
	PollResult *poll(void);

	const std::string& getSessionId(void) const;

	const std::string& getPublisherId(void) const;

	int getMaxPollResultSize(void) const;

	virtual ~ARC();
private:
	friend class SSRC;
	SSRC *_ssrc;

	ARC(const std::string& url,
			const std::string& user,
			const std::string& pass,
			const std::string& capath,
			SSRC *const ssrc);

	ARC(const std::string& url,
			const std::string& mykey,
			const std::string& mykeypw,
			const std::string& mycert,
			const std::string& capath,
			SSRC *const ssrc);
};
}
#endif /* ARC_H_ */
