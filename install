#!/bin/sh
# Copyright (C) 2015 Lukas Himsel; Licensed under GNU GPL v2 (or later)
cc -c toy-http.c
cc -c service.c
cc -o toy-http toy-http.o service.o

rm service.o
rm toy-http.o

sudo cp toy-http /usr/local/bin/
rm toy-http
echo "toy-http installed ..."
