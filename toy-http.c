/*
 * toy-http.c
 * 
 * Copyright 2015 Lukas Himsel <lukas.himsel@web.de>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "service.h"

void help(){
	puts("\033[1;31mtoy-http\033[0m\n--------\n"
		"Usage: \033[1;34mtoy-http\033[0m <PORT> <MAX-CONNECTIONS> <SERVE-FOLDER>\nYou can also use it without any arguments, to run in the actual foler and with a max. of 64 connections.");
}

int main(int argc, char *argv[]){
	if(argc==1){
		printf("\033[1;31mtoy-http\033[0m\n--------\nHost: http://127.0.0.1:%d\n\nCtrl-C to abort.\n", HTTP_PORT);
		return serve(HTTP_PORT, MAX_CONNECTIONS, SERVE_DIRECTORY);
	}
	else if(argc==2){
		if(strcmp(argv[1], "--help")==0 || strcmp(argv[1], "-h")){
			help();
		}
		return 0;
	}
	else if(argc==4){
		printf("\033[1;31mtoy-http\033[0m\n--------\nHost: http://127.0.0.1:%s\n\nCtrl-C to abort.\n", argv[1]);
		return serve(atoi(argv[1]), atoi(argv[2]), argv[3]);
	}
	else{
		puts("error: incompatible arguments");
		help();
		return 1;
	}
	return 0;
}
