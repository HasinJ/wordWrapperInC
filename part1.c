#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<ctype.h>

#ifndef DEBUG
#define DEBUG 0
#endif


void paragraph(char* buff, int i) {
  if(buff[i+1]==10 && buff[i]==10) {
    if (DEBUG) printf("\nparagraph end! ASCIIs: %d %d\n", buff[i], buff[i+1]);
    return;
  }
}

int main(int argc, char const *argv[]) {

  int file = open("sample.txt",O_RDONLY);
  char* buff = malloc(sizeof(char) * 256);

  int bytes_read;
  int bytes_written;

  int i;
  while ((bytes_read = read(file, buff, 256)) > 0) {
  	for (i = 0; i < bytes_read; ++i) {

      if(isspace(buff[i])) {
        paragraph(buff,i);
        bytes_written=write(1,&buff[i],1)
        continue;
      }
      
      bytes_written=write(1,&buff[i],1);
  	}
  }
  free(buff);
  printf("\nbytes_written: %d\n", bytes_written);

  return 0;
}
