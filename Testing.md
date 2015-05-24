# Testing #

## Examples coming with libifmap2c ##
libifmap2c provides a set of example MAPC, which are basically only "Hello World" programs. However, they provide some insight of what is possible with libifmap2c.

If you checked out the source using svn, you'll find a `libifmap2c-examples` folder besides the `libifmap2c` folder. However, you may also download the archive `libifmap2c-examples-<version>.tar.gz`.

The building process is the same as for the library:
  * `cd build`
  * `cmake ../`
  * `make`

**Note:** Some of the programs located in `tests/` use `pthreads`.

Afterwards, you'll find a set of executable files in the `build` folder and  subfolders.
some of them::
  * `utils/ip-mac` utility originally written by Roger Chickering, to publish and delete `ip-mac` metadata, was adapted to use libifmap2c
  * `utils/*` a number of small programs to publish standard metadata and purge them again
  * `tests/*` a number of small programs to test the library and the used MAPS.
  * `example-mapc1` and `example-mapc2`, the MAPC examples from the [Develop](Develop.md) page

All programs use basic authentication, i.e. a username password combination, to authenticate with the MAPS. While libifmap2c provides support for certificate based authentication, basic authentication is easier to set up.

## Open source IF-MAP 2.0 Server ##
To do testing one can use the following open source IF-MAP 2.0 servers:
  * [irond](http://trust.inform.fh-hannover.de/joomla/index.php/downloads/54-iron)
  * [omapd](http://code.google.com/p/omapd/)
  * If you know of more, let me know ;)

## Infoblox IBOS Server ##
You may also try out the IF-MAP Starter Kit offered by Infoblox which includes the IBOS 2.1
server.
  * [IF-MAP Starter Kit](http://www.infoblox.com/en/resources/software-downloads/if-map-starter-kit.html)

The IF-MAP Starter Kit includes a C++ library based on libifmap2c named libqifmap. It t is based on an older version. As libifmap2c had some major bugfixes in the meanwhile, I'd advice you to
use the newest libifmap2c release if possible.

As aside, Akmal Khan tested most of the libifmap2c examples with the Infoblox IBOS 2.0 server. After fixing some defects in libifmap2c, the examples worked with the IBOS server as well.

## The `capath` parameter ##
All test programs listed above require a directory containing the servers certificate in PEM format. This directory needs to be prepared using OpenSSL's `c_rehash` command.

For example, in the build directory execute the following commands:
  * `mkdir capath`
  * `cp /path/to/servercert.pem capath`
  * `c_rehash capath`

## Running the `ip-mac` executable in the `utils/` folder ##
The following command will publish `ip-mac` metadata to the link between
the IP 192.168.0.1 and the MAC 11:bb:cc:dd:ee:ff

`./ip-mac update 192.168.0.1 11:bb:cc:dd:ee:ff https://localhost:8443 test test capath/`

To delete the metadata again, use `delete` instead of `update` as the first
parameter. Or, to delete all metadata of the publisher associated with the username
test:

`./purge myself https://127.0.0.1:8443 test test capath/`

This assumes that you run an IF-MAP server on you local machine with port 8443
to be used for basic authentication, and the `capath` directory was created as described before. Further, test:test can be used as basic autentication credentials.

Alternatively, you may use environment variables to set the url, user, password and capath parameter. See the message when running the examples without sufficient parameters.

The run the other examples, similar parameters are expected. However, there might
be some suprises. It's probably a good idea to read the source code if you are not sure ;-)