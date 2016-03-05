#!/usr/bin/env python
# -*- coding: utf-8 -*-
# script to test the performance of the server
import urllib2
import time

def make_request():
	x=0
	while True:
		urllib2.urlopen("http://127.0.0.1:8976").read()
		x+=1
		print "Request ~%d" % x
		#time.sleep(.100)
	
try:
	make_request()
except KeyboardInterrupt:
	exit()
