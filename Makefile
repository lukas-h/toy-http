# created by Lukas Himsel, 2016
toy-http: toy-http.o
	gcc -O3 -c toy-http.c
	gcc -O3 -o toy-http toy-http.o

diet: toy-http.o
	diet gcc -O2 -c toy-http.c
	diet gcc -O2 -o toy-http toy-http.o

clean:
	rm *.o

install: toy-http
	install -m 0755 toy-http /usr/local/bin

.PHONY: install diet
