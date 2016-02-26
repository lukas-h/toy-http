![](favicon.png)
# toy-http
[![Build Status](https://travis-ci.org/lukas-h/toy-http.svg?branch=one-file)](https://travis-ci.org/lukas-h/toy-http)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](http://www.gnu.org/licenses/agpl-3.0)  
lightweight embeddable http service,
providing static site hosting.  
Not only zero-configuration, also zero dependency alternative to node's [http-server](https://github.com/indexzero/http-server)   
designed for unix-like platforms (Linux, FreeBSD, macOSx, ...)

## features
- support for GET and HEAD requests  
- stable error and interruption management  
- embeddable as one small C function  
- scalable and easy to modify  
- blocking function for parental folders of the serve folder  
- plainness: less than 400 lines of source code  
- no dependencies to external libraries, just the C standard libraries and  
 the posix API (preinstalled on all good unix-derivates) and socket api (included)  
  
I wrote toy-http to test the http protocol and not to make a project
for serious usage. It is now at the point to be a small alternative to
other servers that offer static hosting.
The server is ideal for hosting the current folder to test websites, aso.

## installation
1. download from github.
2. open a terminal and `cd` into the repository
3. type `make` in to compile and link the source code (check the [dependencies](#dependencies))
4. type `sudo make install` to move it 

## usage
After the installation, you can use the command `toy-http`.
#### arguments
If you call the command without any arguments, it'll run in the actual folder.
For more advanced usage, can set as first argument the folder or port. Then the folder and
as third argument how many connections are possible at once.

## files
- *LICENSE*  license file  
- *Makefile*  Makefile to build toy-http from the sources  
- *README.md*  this file
- *favicon.png*  the logo
- *index.html*  test html document
- *install*  alternative Makefile, useful if the program `make` isn't installed, then you can use it by executing `chmod 777 ./install && ./install`
- *service.c*  C source file of the library
- *service.h*  C header of the library
- *start-toy-http*  starter, to execute **toy-http** as a system daemon. It is required to install the program `daemon` to use this script-
- *toy-http.c*  the main source file of the standalone server

## dependencies 
**toy-http** has nearly no dependencies.  
It only needs a unix-like environment with C compiler and the posix API.

#### supported OS:
- (GNU)Linux
- MacOSx
- FreeBSD, OpenBSD, NetBSD, DragonflyBSD
- GNU Hurd
- and every other OS with  
- Solaris (maybe)  
- and more  
Windows is supported with CygWin (or something similar)

#### preferred C compilers:
- gcc
- clang  

## licensing
This project is licensed under the GNU AGPL v3 or later.
Copyright (C) 2015, 2016 Lukas Himsel
