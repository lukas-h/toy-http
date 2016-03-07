#include <stdio.h>
#include <string.h>
#include <time.h>
int parse_head_line(const char *src, char *method, char *filepath){
	if(strlen(src)<5)return 1;
	while(*src && (*method++ = *src++)!=' ');
	while(*src && (*filepath++ = *src++)!=' ');
	return 0;
}
int main(){
	char buf[512] = "GET /home/lukas/index.html", url[256], request[8];
	clock_t start, end, result;
	start = clock();
	
	sscanf(buf, "%7s %255s", request, url);
	
	end = clock();
	result = end - start;
	printf("%f\n", (float)result / CLOCKS_PER_SEC);
	
	
	char buf2[512] = "POST /shia/is/not/famous/anymore", url2[256]="\0", request2[8]="\0";
	start = clock();
	
	if(parse_head_line(buf2, request2, url2)){
		puts("error");
	}
	end = clock();
	puts(request2);
	puts(url2); 
	result = end - start;
	printf("%f\n", (float)result / CLOCKS_PER_SEC);
	return 0;
}
