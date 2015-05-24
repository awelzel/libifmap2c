# libifmap2c - C++ IF-MAP 2.0 Client Library 

## About

libifmap2c is a library written in C++ library to communicate with IF-MAP
2.0 servers. Development started during the IRON project as a personal pet
project. The version back then was limited to newSession and publish
operations. I found some spare time to add the missing functionality and
decided to release it.

## Features

* Easy to write simple IF-MAP 2.0 Clients
* SSRC and ARC channels as objects, e.g. `ssrc->newSession()` / `arc->poll()`
* Uses exceptions for IF-MAP `ErrorResult` (SSRC/ARC) and `EndSessionResult` (ARC)
* No heavy dependencies (based on [libxml2](http://xmlsoft.org/) and [libcurl](http://curl.haxx.se/libcurl/))
* Supports basic and certificate-based authentication

See [Building-and-Installing](Building-and-Installing) and
[Examples-and-Testing](Examples-and-Testing) in the wiki for more
information.
