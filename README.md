![](http://www.lukashimsel.me/pics/toy-http.png)
# toy-http
[![Build Status](https://travis-ci.org/lukas-h/toy-http.svg?branch=master)](https://travis-ci.org/lukas-h/toy-http)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](http://www.gnu.org/licenses/agpl-3.0)  
lightweight embeddable http service,
providing static site hosting.  
## features
- support for GET and HEAD requests (you can implement other methods easily)  
- stable error and interruption management   
- scalable and easy to modify  
- fast file serving  
- blocking function for parental folders of the serve folder  
- less than 500 lines of source code  
- run it as a daemon

I wrote toy-http to test the http protocol and not to make a project
for serious usage. But this server is perfect to for testing websites locally.

## installation
1. download from github.
2. check the [dependencies](#dependencies)
3. open a terminal and `cd` into the repository
4. type `make` in to compile
5. type `sudo make install` to move it  
#### Binary packages
I will add a binary package for Debian- and Ubuntu-based Systems soon! 

## usage
After the installation, you can use the command `toy-http`.
#### arguments
If you call the command without any arguments, it'll run in the actual folder.
For more advanced usage, you can set as first argument the folder or port. Then the folder and
as third argument how many connections are possible at once.  
![](http://www.lukashimsel.me/pics/toy-http2.png)

## dependencies 
no dependencies to external libraries, just the C standard libraries and  
 the posix API (preinstalled on all good unix-derivates) and socket api (included)  

#### preferred C compilers:
- gcc  
- clang  
- tcc

#### supported OS:
- (GNU)Linux  
- Mac OS  
- FreeBSD, OpenBSD, NetBSD, DragonflyBSD  
- GNU Hurd  
- Solaris (maybe)  
- and more, every other OS with POSIX and C libraries  
- Windows is supported with an environment like CygWin (or something similar)

## licensing
This project is licensed under the GNU AGPL v3 or later.  
Copyright (C) 2015, 2016 Lukas Himsel

