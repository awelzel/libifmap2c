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

/*
 * Demonstrates that a IF-MAP session should not be closed, after the TCP
 * connection was dropped.
 */


// libifmap2c includes
#include <libifmap2c/ssrc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <iostream>
#include <cstdlib>
#include <list>


using namespace std;
using namespace ifmap2c;

// needs to be > 0
#define DISCONNECTS 5
#define SLEEP_TIME 2

static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 5)
		usage(argv[0]);

	// create ssrc object which is used for synchronous communication
	SSRC *ssrc = SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);

	string sessionId;

	try {
		cout << "Doing newSession\t\t";
		ssrc->newSession();
		cout << "Ok" << endl;

		// store the session-id for later requests, as we use
		// a new SSRC instance which does not know about it
		sessionId = ssrc->getSessionId();

		for (int i = 0; i < (DISCONNECTS - 1); i++) {

			// Calling the destructor deletes the connection
			cout << "Dropping TCP connection\t\t";
			delete ssrc;
			cout << "Done! (Now sleeping for " << SLEEP_TIME << " seconds)\n";
			sleep(SLEEP_TIME); 

			// the session should still exists.
			// create a new SSRC
			cout << "Creating new SSRC\t\t";
			ssrc =  SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);
			cout << "Done!\n";

			// do a renewsession request, explicitly set the
			// sessionId, as we don't run a newSession on this
			// SSRC.
			cout << "Running renewSession\t\t";
			ssrc->renewSession(sessionId);
			cout << "Done!\n";
		}

		// Disconnect and sleep for the last time
		cout << "Dropping TCP connection\t\t";
		delete ssrc;
		cout << "Done! (Now sleeping for " << SLEEP_TIME << " seconds)\n";
		sleep(SLEEP_TIME); 
		
		cout << "Creating new SSRC\t\t";
		ssrc =  SSRC::createSSRC(argv[1], argv[2], argv[3],argv[4]);
		cout << "Done!\n";

		cout << "Doing endSession\t\t";
		ssrc->endSession(sessionId);
		cout << "Ok" << endl;

	} catch (IfmapError e) {
		cerr << e << endl;
	} catch (ErrorResult e) {
		cerr << e << endl;
	}

	// delete the publish requests and all their childs
	delete ssrc;
	return 0;
}
