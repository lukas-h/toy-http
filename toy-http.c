/*
 * toy-http.c
 *
 * Copyright 2015, 2016 Lukas Himsel <lukas.himsel@web.de>
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

/* --- defaults & definitions --- */
#define DEFAULT_FILE	"index.html"
#define HTTP_PORT	8976
#define MAX_CONNECTIONS	1024
#define SERVE_DIRECTORY	"."
#define FILE_CHUNK_SIZE	4096U

/* --- HTTP messages --- */
const char *HTTP_ERR_501 = "HTTP/1.0 501 Not Implemented\r\nContent-Type: text/html\r\nContent-length: 104\r\n\r\n"
	"<html><head><title>Error</title></head><body><hr><h1>HTTP method not implemented.</h1><hr><p>toy-http</p></body></html>";

const char *HTTP_ERR_404 = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\nContent-length: 91\r\n\r\n"
	"<html><head><title>Error</title></head><body><hr><h1>File not found.</h1><hr><p>toy-http</p></body></html>";

const char *HTTP_ERR_403 = "HTTP/1.0 403 Forbidden\r\nContent-Type: text/html\r\nContent-length: 91\r\n\r\n"
	"<html><head><title>Error</title></head><body><hr><h1>No permission.</h1><hr><p>toy-http</p></body></html>";

/* --- storage for mime types --- */
struct key_val_t{
	char *key;
	char *val;
} const array[] ={
	{ ".html",  "text/html"			},
	{ ".htm" ,  "text/html"			},
	{ ".css" ,  "text/css"			},
	{ ".js"  ,  "text/javascript"	},
	{ ".json",  "application/json"	},
	{ ".pdf" ,  "application/pdf"	},
	{ ".png" ,  "image/png"			},
	{ ".jpg" ,  "image/jpeg"		},
	{ ".svg" ,  "image/svg+xml"		},
	{ ".gif" ,  "image/gif"			}
};

static int parse_args(int argc, char *argv[]);
static int is_numeric(char *str);
static void inline help(void);

static ssize_t file_attributes(char *filename);
static char *get_content_type(char *filename);
int parse_head_line(const char *src, char *method, char *filepath);
static ssize_t recv_line(int fd, char *buf, size_t len);
static int serve(int client);

/* --- globals --- */
static int http_port = HTTP_PORT;
static int max_connections = MAX_CONNECTIONS;
static int serve_dir = 0;

/* --- error and signal handling --- */
#define error(msg, ...)		fprintf(stderr, "\033[1;41merror:\033[0m" msg, ##__VA_ARGS__)
#define warning(msg, ...)	fprintf(stderr, "\033[1;43mwarning:\033[0m" msg, ##__VA_ARGS__)
#define info(msg, ...)		fprintf(stdout, "\033[1;44minfo:\033[0m" msg, ##__VA_ARGS__)

#define SET_FD 1
#define CLOSE_FD 2
static void handle_fd(int fd, int instr){
	static int f;
	switch(instr){
		case SET_FD:
			f=fd? fd : -1;
		break;
		case CLOSE_FD:
			close(f);
		break;
	}
}
static void abort_program(int signum){
	handle_fd(0, CLOSE_FD);

	switch(signum){
		case SIGABRT: case SIGHUP:
			error("abnormal termination: exit");
		break;
		case SIGILL: case SIGSEGV:
			error("invalid instruction: please contact the maintainer");
		break;
		case SIGINT:
			info("exit");
		break;
		case SIGTERM: case SIGKILL:
			info("termination request: exit");
			signum = 0;
		break;
	}

	_exit((signum > 0) ? 1 : 0);
}

/* --- SERVER MAIN --- */
int main(int argc, char *argv[]){
	char *serve_directory = ".";

	int fd, client, pid;
	struct sockaddr_in addr, client_addr;
	socklen_t siz;

	if(parse_args(argc, argv)){
		return 1;
	}

	printf("\033[1;31mtoy-http\033[0m\n--------\n");

	signal(SIGABRT, abort_program); //signal handling
	signal(SIGILL, abort_program);
	signal(SIGINT, abort_program);
	signal(SIGSEGV, abort_program);
	signal(SIGTERM, abort_program);
	signal(SIGCHLD, SIG_IGN);

	if(http_port > 65535){
		error("illegal port\n");
		abort_program(-1);
	}

	if(serve_dir>0){
		serve_directory = argv[serve_dir];
	}

	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(fd == -1){
		error("can not create new socket\n");
		abort_program(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(http_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))==-1){
		error("bind() failed.\n >Is another server running on this port?\n");
		abort_program(-1);
	}

	if(listen(fd, max_connections)==-1){
		error("listen() failed\n");
		abort_program(-1);
	}

	if(chdir(serve_directory)){
		error("invalid path or can not set\n");
		abort_program(-1);
	}

	info("Server running!\n"
	" >host: http://127.0.0.1:%d\n"
	" >\033[1mCtrl-C\033[0m to abort.\n", http_port);
	handle_fd(fd, SET_FD);

	while(1){
		siz = sizeof(struct sockaddr_in);
		client = accept(fd, (struct sockaddr *)&client_addr, &siz);

		if(client==-1){
			error("accept() failed\n");
			continue;
		}

		pid = fork();
		if(pid==-1){
			error("fork() failed\n");
			continue;
		}
		if(pid==0){
			close(fd);
			serve(client);

			shutdown(client, SHUT_RDWR);
			close(client);

			return 0;
		}
		close(client);
	}
	close(fd);

	return 0;
}

/* --- argument parser & help display --- */
static int parse_args(int argc, char *argv[]){
	size_t num_count, path_count, i;
	if(!(argc > 0)){
		return 1;
	}
	for(i=1, num_count=0, path_count=0; i < argc && i < 5; i++){
		if(strcmp(argv[i], "--help")==0 || strcmp(argv[i], "-h")==0){
			help();
			return 1;
		}
		else if(is_numeric(argv[i])){
			switch(num_count){
				case 0:
					http_port = atoi(argv[i]);
				break;
				case 1:
					max_connections = atoi(argv[i]);
				break;
				default:
					help();
					return 1;
				break;
			}
			num_count++;
		}
		else{
			if(path_count > 0){
				help();
				return 1;
			} else{
				serve_dir = i;
			}
			path_count++;
		}
	}

	return 0;
}

static int is_numeric(char *str){
	do{
		if( !(*str>='0' && *str<='9') ){ return 0; }
		str++;
	} while(*str);

	return 1;
}

static inline void help(){
	printf(
		"Usage: \033[1;31mtoy-http\033[0m [PORT] [SERVE-FOLDER] [MAX-CONNECTIONS]\n"
		"You can also use it without any arguments,\n to run it in the actual folder.\n\n"
		"See the project page: `http://himsel.me/toy-http'\n"
		"Copyright (C) 2015, 2016 Lukas Himsel\n"
	    "  This program comes with ABSOLUTELY NO WARRANTY.\n"
	    "  This is free software, and you are welcome to redistribute it\n"
	    "  under certain conditions; see `www.gnu.org/licenses/agpl.html\' for details.\n"
	);
}

/* --- server procedures and file handling --- */
#define NOT_EXISTING -1
#define FILE_IS_DIR  -2
#define FILE_NO_PERM -3

static ssize_t file_attributes(char *filename){
	struct stat info;

	if(strstr(filename, "..")!=NULL || filename[0]=='/'){ return FILE_NO_PERM; }
	if(stat(filename, &info)==-1){ return NOT_EXISTING; }
	if(!(info.st_mode & S_IRUSR)){ return FILE_NO_PERM; }
	if(S_ISDIR(info.st_mode)){ return FILE_IS_DIR; }

	return info.st_size;
}

static char *get_content_type(char *filename){
	size_t index, len_name, len_key;
	len_name = strlen(filename);
	char *p;
	for(index=0; index < sizeof(array) / sizeof(struct key_val_t); index++){
		p = array[index].val;
		len_key = strlen(array[index].key);

		if(len_key > len_name){
			continue;
		} else{
			if(strcmp(array[index].key, &(filename[len_name - len_key]))==0){
				return p;
			}
		}
	}

	return NULL;
}

static ssize_t recv_line(int fd, char *buf, size_t len){
	size_t i=0;
	ssize_t err=1;
	while((i < len-1) && err==1){
		err = recv(fd, &(buf[i]), 1, 0);
		if(buf[i]=='\n'){ break; }
		else{ i++; }
	}
	if(i && (buf[i-1]=='\r')){ i--; }
	buf[i] = '\0';

	return i;
}

int parse_head_line(const char *src, char *method, char *filepath){
	if(strlen(src)<5)return 1;
	while((*method++ = *src++) && *src!=' ');

	src++; /* whitespace */
	while((*filepath++ = *src++) && *src!=' ');
	return 0;
}

static int serve(int client){
	char buf[FILE_CHUNK_SIZE]="\0", request[8]="\0", url[256]="\0";
	char *filename, *content_type;
	ssize_t len, file_size;
	FILE *f;

	if(recv_line(client, buf, (sizeof(buf)-1) )<=3){
		warning("can not receive request\n");
		return 1;
	}
	if(parse_head_line(buf, request, url)){
		warning("parsing error\n >request: `%s`\n", buf);
		return 1;
	}

	while(recv_line(client, buf, (sizeof(buf)-1) ) > 0);

	if( (strcmp(request, "GET")!=0) && (strcmp(request, "HEAD")!=0)){

		send(client, HTTP_ERR_501, strlen(HTTP_ERR_501), 0);

		warning("request method not supported\n >method: `%s`\n", request);
		return 0;
	}

	if(url[strlen(url)-1]=='/'){
		url[strlen(url)-1]='\0';
	}

	filename = &(url[1]);
	if(!strlen(filename)){ filename = DEFAULT_FILE;	}

	file_size = file_attributes(filename);

	if(file_size==FILE_IS_DIR){
		strcat(filename, "/index.html");
		file_size = file_attributes(filename);
	}
	if(file_size==NOT_EXISTING){
		send(client, HTTP_ERR_404, strlen(HTTP_ERR_404), 0);
		return 0;
	}
	if(file_size==FILE_NO_PERM){
		send(client, HTTP_ERR_403, strlen(HTTP_ERR_403), 0);
		return 0;
	}

	f = fopen(filename, "r");
	if(f==NULL){
		send(client, HTTP_ERR_404, strlen(HTTP_ERR_404), 0);
		return 0;
	}

	send(client, "HTTP/1.0 200 OK\r\n", 17, 0);

	/* add content information */
	content_type = get_content_type(filename);
	if(content_type!=NULL){
		sprintf(buf, "Content-type: %s\r\n", content_type);
		send(client, buf, strlen(buf), 0);
	} else{
		warning("No match for file extension `%s`.\n >Sending response without Content-type", filename);
	}

	sprintf(buf, "Content-length: %ld\r\nServer: toy-http\r\n\r\n", file_size);
	send(client, buf, strlen(buf), 0);

	if(strcmp(request, "GET") == 0){
		while(!feof(f)){
			len = fread(buf, 1, FILE_CHUNK_SIZE, f);

			if(len){ send(client, buf, len, 0); }
		}
	}
	fclose(f);

	return 0;
}
