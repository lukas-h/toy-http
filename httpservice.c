#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <signal.h>

#define MAX_CONNECTIONS  20
#define PORT             8976
#define SERVE_DIRECTORY  "."
#define DEFAULT_FILE     "index.html"

static void exit_err(char *msg){
	fprintf(stderr, "error: %s", msg);
	exit(1);
}

int get_line(int fd, char *buf, int len){
	unsigned int i=0;
	while((i < len-1) && (recv(fd, &(buf[i]), 1, 0) == 1)){
		if(buf[i]=='\n'){ break; }
		else{ i++; }
	}
	if((i > 0) && (buffer[i-1] == '\r')){
		i--;
	}
	buffer[i] = '\0';
	return i;
}

int is_html(char *filename){
	
}
