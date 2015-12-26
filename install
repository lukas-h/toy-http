#!/bin/sh
# Copyright (C) 2015 Lukas Himsel; Licensed under GNU GPL v2 (or later)
gcc -c toy-http.c
gcc -c service.c
gcc -o toy-http toy-http.o service.o

rm service.o
rm toy-http.o

sudo cp toy-http /usr/local/bin/
rm toy-http
echo "toy-http installed ..."
