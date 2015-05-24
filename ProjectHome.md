# About libifmap2c #
libifmap2c is a library written in C++ library to communicate with [IF-MAP 2.0](http://www.trustedcomputinggroup.org/resources/tnc_ifmap_binding_for_soap_specification) servers. Development started during the [IRON](http://trust.inform.fh-hannover.de/joomla/index.php/projects/iron) project as a personal pet project. The version back then was limited to _newSession_ and _publish_ operations. I found some spare time to add the missing functionality and decided to release it.

# Features #
  * Very easy to write simple IF-MAP 2.0 Clients
  * SSRC and ARC channels as objects, e.g. ssrc->newSession() / arc->poll()
  * Uses exceptions for IF-MAP ErrorResult (SSRC/ARC) and EndSessionResult (ARC)
  * No heavy dependencies (based only on [libxml2](http://xmlsoft.org/) and [libcurl](http://curl.haxx.se/))
  * Supports basic and certificate-based authentication


See the Wiki pages for instructions how to [install](Install.md), [test](Testing.md) and
[develop](Develop.md).

If you find bugs, memory leaks or errors please notify me about them. I'm also
happy to discuss general questions.

Thanks, Arne

# News #
## libifmap2c pre-0.4.0 Release ##
(28. July 2012)

So there have been quite some changes since 0.3.0 in trunk, but I've just been too busy lately to make a real release. However, I added archives for the 0.4.0-pre version on the download page. It builds and runs on Ubuntu Precise, Slackware 13.37 and OpenBSD 5.0 and should be fairly stable. In case you stumble upon problems, just let me know.

While most of the changes from 0.3.0 to 0.4.0 are internally, 0.4.0 changes the interface to the user again. These changes are listed in the ChangeLog fil. Further I fixed up the examples, so that should hopefully give clues on how to solve breakage...

Download the archives [here](http://code.google.com/p/libifmap2c/downloads/list).

Happy Coding!

## libifmap2c 0.3.0 Release ##
(10. July 2011)

> The "Lets break things" release ;)

This release breaks some parts of the 0.2.0 interface, but I think the changes are
worth doing and, while maybe tedious, adapting old code should not be too hard.

Examples now include a `utils/` folder which contains small programs to publish standard metadata. They can be used to do some easy testing. It might even be possible to script a _real MAPC_ by using some of them together.

See [here](LibraryChanges030.md) for changes to the library, and [there](ExamplesChanges030.md) for changes to the examples package.

Download the archives [here](http://code.google.com/p/libifmap2c/downloads/list).

Happy Coding!