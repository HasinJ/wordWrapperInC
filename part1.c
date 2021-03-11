#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<ctype.h>
#include<assert.h>

#ifndef DEBUG
#define DEBUG 0
#endif


void paragraph(char* buff, int i) {
  if(buff[i+1]==10 && buff[i]==10) {
    if (DEBUG) printf("\nparagraph end! ASCIIs: %d %d\n", buff[i], buff[i+1]);
    return;
  }
}

void storeWord(char* word, char* buff, int wordStart, int end, int* stored,int* pos, int bytes_read, int* word_length) {
  size_t i;
  for (i = wordStart; i < end; i++) {
    word[*pos]=buff[i];
    (*pos)++;
  }
  *stored=1;
}

void printBuffer(char* buff, int buff_length){
  size_t i;
  for (i = 0; i < buff_length; i++) {
    printf("position: %ld letter: %c\n", i, buff[i]);
  }
}


void exceeded(int output_fd, int* bytes_written, char* buffer, int start, int end) {
  if (DEBUG) printf(" width exceeded\n");
  write(output_fd,"\n",1);
  bytes_written=0;
  bytes_written+=write(1,&buffer[++start],--end);
}

void writeRegular(int output_fd, int* bytes_written, char* buff, int start, int end) {
  *bytes_written+=write(output_fd,&buff[start],end);
}

void output(int* bytes_written, char* buff, int start, int end, int length, unsigned width, int output_fd){
  if(*bytes_written+length>width) {
    if (DEBUG) printf(" width exceeded\n");
    write(output_fd,"\n",1);
    *bytes_written=0;
    start++;
    end--;
  }
  *bytes_written+=write(output_fd,&buff[start],end);

}


void wrap(unsigned width, int input_fd, int output_fd){
  size_t i=0;
  int buff_length = 4;
  char* buff = malloc(sizeof(char) * buff_length);
  int bytes_read=0, bytes_written=0,wasSpace=0,wordStart=0;

  while ((bytes_read = read(input_fd, buff, buff_length)) > 0) {
    if (DEBUG) printBuffer(buff,buff_length);
  	for (i = 0; i < bytes_read; ++i) {
      if(isspace(buff[i])){
        if(i<=buff_length-2) paragraph(buff,i);
        if(!wasSpace) {
          wasSpace=1; //wasSpace doesnt exists
          wordStart=i;
          if (DEBUG) printf("//wasSpace doesnt exists\n");
        }
        else {
          if (DEBUG) printf("//wasSpace does exist\n");
          output(&bytes_written, buff, wordStart, i, i-wordStart, width, output_fd); //bytes_written+=write(output_fd,&buff[wordStart],i); //wasSpace does exist
          wordStart=i;
        }
        if(i==buff_length-1) bytes_written=write(output_fd," ",1);
      }
  	}
    if (!wasSpace) {
      if (DEBUG) printf("\n//just write\n");
      output(&bytes_written, buff, 0, i, i, width, output_fd);
      //bytes_written+=write(output_fd,&buff[0],i);
    }
    wasSpace=0;
    //if(isspace(buff[buff_length-1])) bytes_written+=write(output_fd," ",1);
    if(DEBUG) printf("\nbytes_read: %d\n", bytes_read);
    if(DEBUG) printf("bytes_written: %d\n", bytes_written);
    free(buff);
    buff = malloc(sizeof(char) * buff_length);
  }
  free(buff);

  //free(word);
}



int main(int argc, char const *argv[]) {

  if (argc>2 || argc==1) {
    printf("Please enter just the width for now.\n");
    return EXIT_SUCCESS;
  }

  int fileDsc = open("sample.txt",O_RDONLY);
  unsigned width=atoi(argv[1]);
  wrap(width,fileDsc,1);

  close(fileDsc);
  if(DEBUG) printf("width: %d\n", width);

  return 0;
}
