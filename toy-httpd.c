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

void help(){
	puts("\033[1;31mtoy-http\033[0m\n--------\n"
		"Usage: \033[1;34mtoy-http\033[0m <PORT> <SERVE-FOLDER> <MAX-CONNECTIONS>\n"
		"You can also use it without any arguments,\n to run in the actual folder.");
}

void version(){
	puts("\033[1;31mtoy-http\033[0m\n--------\n"
		"Version: 0.0\nCopyright (C) 2015 Lukas Himsel\nlicensed under GNU AGPL v3\n"
	    "This program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it\n"
	    "under certain conditions; see `www.gnu.org/licenses/gpl.html\' for details.\n");
}

int is_numeric(char *str);
int is_path(char *str);

int main(int argc, char *argv[]){
	int err=0;

	/*
	if(argc==1){
		err=serve(HTTP_PORT, MAX_CONNECTIONS, SERVE_DIRECTORY);
	}
	else if(argc==2){
		if(strcmp(argv[1], "--help")==0){
			help();
		}
		else if(strcmp(argv[1], "--version")==0){
			version();
		}
		else if(is_numeric(argv[1])){
			err=serve(atoi(argv[1]), MAX_CONNECTIONS, SERVE_DIRECTORY);
		}
		else if(is_path(argv[1])){
			err=serve(HTTP_PORT, MAX_CONNECTIONS, argv[1]);
		}
		else{
			puts("error: incompatible arguments");
			help();
			err=1;
		}
	}
	else if(argc==3){
		if(is_numeric(argv[1]) && is_path(argv[2])){
			err=serve(atoi(argv[1]), MAX_CONNECTIONS, argv[2]);
		}
	}
	else if(argc==4){
		if(is_numeric(argv[1]) && is_path(argv[2]) && is_numeric(argv[1])){
			err=serve(atoi(argv[1]), atoi(argv[2]), argv[3]);
		}
	}
	else{
		puts("error: incompatible arguments");
		help();
		err=1;
	}*/

	switch(argc){

		case 1:
			err=serve(HTTP_PORT, MAX_CONNECTIONS, SERVE_DIRECTORY);
		break;

		case 2:
			if(strcmp(argv[1], "--help")==0){
				help();
			}
			else if(strcmp(argv[1], "--version")==0){
				version();
			}
			else if(is_numeric(argv[1])){
				err=serve(atoi(argv[1]), MAX_CONNECTIONS, SERVE_DIRECTORY);
			}
			else if(is_path(argv[1])){
				err=serve(HTTP_PORT, MAX_CONNECTIONS, argv[1]);
			}
			else{
				puts("error: incompatible arguments");
				help();
				err=1;
			}
		break;

		case 3:
			if(is_numeric(argv[1]) && is_path(argv[2])){
				err=serve(atoi(argv[1]), MAX_CONNECTIONS, argv[2]);
			}
		break;

		case 4:
			if(is_numeric(argv[1]) && is_path(argv[2]) && is_numeric(argv[1])){
				err=serve(atoi(argv[1]), atoi(argv[2]), argv[3]);
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

int is_numeric(char *str){
	while(*str++){
		if(!isdigit(*str))
			return 0;
	}
	return 1;
}

int is_path(char *str){
	int val;
	do{
		if(isalpha(*str)){ val++; }
		if(*str=='/'){ val++; }
		if(*str=='.'){ val+=2; }
	} while(*str++);
	return (val > 2) ? 1 : 0;
}
