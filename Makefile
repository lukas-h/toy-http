# created by Lukas Himsel, 2016
toy-http: toy-http.o
	cc -O2 -c toy-http.c
	cc -O2 -o toy-http toy-http.o

clean:
	rm *.o

install: toy-http
	install -m 0755 toy-http /usr/local/bin

.PHONY: install
