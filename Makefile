# Copyright (C) 2015 Lukas Himsel; Licensed under GNU GPL v2 (or later)
toy-http: toy-http.o service.o
	cc -c toy-http.c
	cc -c service.c
	cc -o toy-http toy-http.o service.o

clean:
	rm *.o

install: toy-http
	install -m 0755 toy-http /usr/local/bin

.PHONY: install
