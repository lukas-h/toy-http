/*
 * service.c
 *
 * Copyright 2015 Lukas Himsel <lukas.himsel@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include "service.h"

static long file_attributes(char *filename);
static int recv_line(int fd, char *buf, int len);
static int ishtml(char *filename);
static int iscss(char *filename);
static int isjavascript(char *filename);
static int http_service(int client);


#define SET_FD 1
#define CLOSE_FD 2
static void getter_fd(int fd, int client, int instr){
	static int f1, f2;
	
	if(instr==SET_FD){
		if(fd){
			f1=fd;
		}
		if(client){
			f2=client;
		}
	}
	if(instr==CLOSE_FD){
		close(f1);
		shutdown(f2, SHUT_RDWR);
		close(f2);
	}
}

static void abort_program(int signum){
	getter_fd(0, 0, CLOSE_FD);
	
	switch(signum){
		case SIGABRT:
			fprintf(stderr, "abnormal termination: exit\n");
			exit(1);
		break;
		case SIGILL:
			fprintf(stderr, "invalid instruction: please contact the maintainer\n");
			exit(1);
		break;
		case SIGINT:
			exit(0);
		break;
		case SIGSEGV:
			fprintf(stderr, "invalid memory access: please contact the maintainer\n");
			exit(1);
		break;
		case SIGTERM:
			fprintf(stderr, "termination request: exit\n");
		break;
		default:
			exit(1);
		break;
	}
	exit(1); // sicherheitshalber
}

int serve(int http_port, int max_connections, char *serve_directory){
	int fd, client, pid, err_cnt;
	struct sockaddr_in addr, client_addr;
	socklen_t siz;
	
	signal(SIGABRT, abort_program); //signal handling
	signal(SIGILL, abort_program);
	signal(SIGINT, abort_program);
	signal(SIGSEGV, abort_program);
	signal(SIGTERM, abort_program);

	if(http_port >  65535){
		fprintf(stderr, "error: illegal port\n");
		return PORT_ERR;
	}
	
	fd = socket(PF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		fprintf(stderr, "error: can not create new socket\n");
		return SOCKET_ERR;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(http_port);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))==-1){
		fprintf(stderr, "error: bind() failed.\n\033[1;31mIs another server running on this port?\033[0m\n");
		return BIND_ERR;
	}
	
	if(listen(fd, max_connections)==-1){
		fprintf(stderr, "error: listen() failed\n");
		return LISTEN_ERR;
	}
	
	if(chdir(serve_directory)){
		fprintf(stderr, "error: invalid path or can not set\n");
		return PATH_ERR;
	}
	
	while(1){
		
		siz = sizeof(struct sockaddr_in);
		client = accept(fd, (struct sockaddr *)&client_addr, &siz);
		
		getter_fd(fd, client, SET_FD); //signal handling
		
		if(client==-1){
			fprintf(stderr,"error: accept() failed\n");
			err_cnt++;
			if(err_cnt > 2){
				return ACCEPT_ERR;
			}
			continue;
		} else{
			err_cnt=0;
		}
		
		pid = fork();
		if(pid==-1){
			fprintf(stderr, "error: fork() failed\n");
			err_cnt++;
			if(err_cnt > 2){
				close(client);
				return FORK_ERR;
			}
			continue;
		}
		if(pid==0){
			close(fd);
			
			http_service(client);
			
			shutdown(client, SHUT_RDWR);
			close(client);
			return 0;
		}
		close(client);
	}
	
	close(fd);
	return 0;
}


#define NOT_EXISTING  0
#define FILE_IS_DIR  -1
#define FILE_NO_PERM -2

static long file_attributes(char *filename){
	struct stat info;
	if(stat(filename, &info)==-1){
		return 0;
	}
	
	return info.st_size;
}

static int recv_line(int fd, char *buf, int len){
	unsigned int i=0, err=1;
	while((i < len-1) && err==1){
		err = recv(fd, &(buf[i]), 1, 0);
		if(buf[i]=='\n'){ break; }
		else{ i++; }
	}
	if((i > 0) && (buf[i-1]=='\r')){
		i--;
	}
	buf[i] = '\0';
	return i;
}

static int ishtml(char *filename){
	if(strcmp( &(filename[strlen(filename)-4]), ".htm")==0 || strcmp( &(filename[strlen(filename)-5]), ".html")==0){
		return 1;
	}
	return 0;
}
static int iscss(char *filename){
	if(strcmp( &filename[strlen(filename)-4], ".css")==0){
		return 1;
	}
	return 0;
}
static int isjavascript(char *filename){
	if(strcmp( &filename[strlen(filename)-3], ".js")==0){
		return 1;
	}
	return 0;
}

static int http_service(int client){
	char buf[256], request[8], url[128];
	char *filename;
	int len;
	long file_size;
	FILE *f;
	
	if(recv_line(client, buf, 256)==0){
		fprintf(stderr, "warning: can not receive request\n");
		return 1;
	}
	if(sscanf(buf, "%7s %127s", request, url) < 2){
		fprintf(stderr, "warning: parsing error %s\n", buf);
		return 1;
	}
	
	while(recv_line(client, buf, 256) > 0);
	
	if( (strcmp(request, "GET")!=0) && (strcmp(request, "HEAD")!=0)){
		
		send(client, "HTTP/1.0 501 Not Implemented\r\nContent-Type: text/html\r\nContent-length: 104\r\n\r\n"
			"<html><head><title>Error</title></head><body><hr><h1>HTTP method not implemented.</h1><hr></body></html>", 162, 0);
		
		fprintf(stderr, "warning: request method `%s` not supported\n", request);
		return 0;
	}

	if(url[strlen(url)-1]=='/'){
		url[strlen(url)-1]='\0';
	}
	
	filename = &(url[1]);
	if(!strlen(filename)){
		filename = DEFAULT_FILE;
	}

	file_size = file_attributes(filename);
	if(file_size==NOT_EXISTING){
		send(client, "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-length: 91\r\n\r\n"
			"<html><head><title>Error</title></head><body><hr><h1>File not found.</h1><hr></body></html>", 162, 0);
		return 0;
	}
	else if(file_size==FILE_IS_DIR){
		sprintf(url, "%s/%s", url, DEFAULT_FILE);
		filename = &(url[1]);
	}
	else if(file_size==FILE_NO_PERM){
		send(client, "HTTP/1.0 403 Forbidden\r\nContent-Type: text/html\r\nContent-length: 91\r\n\r\n"
			"<html><head><title>Error</title></head><body><hr><h1>No permission.</h1><hr></body></html>", 162, 0);
		return 0;
	}
	
	f = fopen(filename, "r");
	if(f==NULL){
		send(client, "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-length: 91\r\n\r\n"
			"<html><head><title>Error</title></head><body><hr><h1>File not found.</h1><hr></body></html>", 162, 0);
		return 0;
	}
	
	send(client, "HTTP/1.0 200 OK\r\n", 17, 0);
	
	/* add content information */
	if(ishtml(filename)){
		send(client, "Content-type: text/html\r\n", 25, 0);
	}
	else if(iscss(filename)){
		send(client, "Content-type: text/css\r\n", 31, 0);
	}
	else if(isjavascript(filename)){
		send(client, "Content-type: text/javascript\r\n", 24, 0);
	}
	
	sprintf(buf, "Content-length: %ld\r\n\r\n", file_size);
	send(client, buf, strlen(buf), 0);
	
	/* if not only HEAD */
	if(strcmp(request, "GET") == 0){
		while(!feof(f)){
			len = fread(buf, 1, 256, f);
			if(len){
				send(client, buf, len, 0);
			}
		}
	}
	fclose(f);
	return 0;
}
