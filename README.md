![](http://file.himsel.me/img/toy-http.png)
# toy-http
[![Build Status](https://travis-ci.org/lukas-h/toy-http.svg?branch=master)](https://travis-ci.org/lukas-h/toy-http)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](http://www.gnu.org/licenses/agpl-3.0)  
Example http server, providing static file hosting.  
See the [project page (himsel.me/toy-http)](http://www.lukashimsel.me/toy-http/)
## features
- support for GET and HEAD requests (you can implement other methods easily)  
- stable error and interruption management   
- scalable and easy to modify  
- fast file serving  
- blocking function for parental folders of the serve folder  
- less than 500 lines of source code

## installation
1. download from github.
2. check the [dependencies](#dependencies)
3. open a terminal and `cd` into the repository
4. type `make` to compile
5. and then `sudo make install` 

#### Binary packages
download [here](http://www.lukashimsel.me/projects.html#bin-toy-http)

## usage
type `toy-http` into a terminal!

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

