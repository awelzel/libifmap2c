// libifmap2c includes
#include <libifmap2c/ssrc.h>

#include <iostream>

#include <cstdlib>

#include "common.h"

using namespace std;
using namespace ifmap2c;

static void
usage(const char *const name)
{
	cerr << "Usage: " << name << " ifmap-server-url username password capath" << endl;
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *url, *user, *pass, *capath;
	
	checkAndLoadParameters(argc, argv, 0, usage, &url, &user,
			&pass, &capath);

	SSRC *ssrc = SSRC::createSSRC(url, user, pass, capath);
	
	try {	
		// if the host is offline, this should throw
		// an XmlCommunicationError error
		ssrc->newSession();
	} catch (const XmlCommunicationError &e) {
		cout << "XmlCommunicationError:type=" <<
			e.getErrorType() << ", msg=" <<
			e.getMessage() <<
			endl;

	} catch (const ErrorResult &e) {
		cerr << e << endl;
	}

	// delete the publish requests and all their childs
	delete ssrc;
	return 0;
}
