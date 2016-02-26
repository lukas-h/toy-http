/*
 * toyhttp.c
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
#include <ctype.h>

#include "service.h"

int run_server(int port, int max_connections, char *serve_dir);
int is_numeric(char *str);
int helppages(char *str);
void help(void);

int main(int argc, char *argv[]){
	int err=0;


	switch(argc){
		case 1:
			err=run_server(HTTP_PORT, MAX_CONNECTIONS, SERVE_DIRECTORY);
		break;
		case 2:
			if( !helppages(argv[1])){
				if(is_numeric(argv[1])){
					err=run_server(atoi(argv[1]), MAX_CONNECTIONS, SERVE_DIRECTORY);
				}
				else if(!is_numeric(argv[1])){
					err=run_server(HTTP_PORT, MAX_CONNECTIONS, argv[1]);
				}
				else{
					puts("error: incompatible arguments");
					help();
					err=1;
				}
			}
		break;
		case 3:
			if(is_numeric(argv[1]) && !is_numeric(argv[2])){
				err=run_server(atoi(argv[1]), MAX_CONNECTIONS, argv[2]);
			}
		break;
		case 4:
			if(is_numeric(argv[1]) && !is_numeric(argv[2]) && is_numeric(argv[3])){
				err=run_server(atoi(argv[1]), atoi(argv[3]), argv[2]);
			}
		break;
		default:
			puts("error: incompatible arguments");
			help();
			err=1;
		break;
	}

	return err;
}

int run_server(int port, int max_connections, char *serve_dir){
	// status message
	printf("\033[1;31mtoy-http\033[0m\n--------\nHost: http://127.0.0.1:%d\n\nCtrl-C to abort.\n", port);
	return serve(port, max_connections, serve_dir);
}

int is_numeric(char *str){
	do{
		if( !(*str>='0' && *str<='9')){
			return 0;
		}
		str++;
	} while(*str);
	return 1;
}

void help(){
	puts("\033[1;31mtoy-http\033[0m\n--------\n"
	"Usage: \033[1;34mtoy-http\033[0m <PORT> <SERVE-FOLDER> <MAX-CONNECTIONS>\n"
	"You can also use it without any arguments,\n to run it in the actual folder.");
}

int helppages(char *str){
	if(strcmp(str, "--help")==0 || strcmp(str, "-h")==0){
		help();
		return 1;
	}
	else if(strcmp(str, "--version")==0 || strcmp(str, "-v")==0){
		puts("\033[1;31mtoy-http\033[0m\n--------\n"
		"Version: 0.0\nCopyright (C) 2015 Lukas Himsel\nlicensed under GNU AGPL v3\n"
	    "This program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it\n"
	    "under certain conditions; see `www.gnu.org/licenses/gpl.html\' for details.\n");
	    return 1;
	}
	else{
		return 0;
	}
}
