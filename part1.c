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



void printBuffer(char* buff, int buff_length){
  size_t i;
  for (i = 0; i < buff_length; i++) {
    printf("position: %ld letter: %c ASCII: %d\n", i, buff[i], buff[i]);
  }
}


void writeStored(int* bytes_written, int lastWord, char* buff, int start, int end, int length, unsigned width, int output_fd){
  int fullLength = *bytes_written+length;
  if(*bytes_written!=0) fullLength++;

  if(fullLength>width) {
    write(output_fd,"\n",1);
    *bytes_written=0;
    if(isspace(buff[start])){
      if (DEBUG) printf("BOOM bug\n"); //no space exists
    }
  }

  if(*bytes_written!=0) *bytes_written+=write(output_fd," ",1);
  *bytes_written+=write(output_fd,&buff[start],end);

}



void wrap(unsigned width, int input_fd, int output_fd){
  size_t i=0;
  int buff_length = 2;
  int word_length = buff_length;
  char* buff = malloc(sizeof(char) * buff_length);
  char* word = calloc(word_length,sizeof(char));
  int bytes_read=0, bytes_written=0,pos=0,lastWord=0,newLine=0,newParagraph=0;

  while ((bytes_read = read(input_fd, buff, buff_length)) > 0) {
    if (DEBUG) printBuffer(buff,buff_length); //paragraph(buff, i, output_fd, &bytes_written)

  	for (i = 0; i < bytes_read; ++i) {
      if(isspace(buff[i])) {
        if(pos) {
          if (DEBUG) printf("dumping word...\n");
          writeStored(&bytes_written,lastWord,word,0,pos,pos,width,output_fd);
          lastWord=1;
          free(word);
          word=calloc(word_length,sizeof(char));
          pos=0;
        }
        if(buff[i]==10){
          if(!newLine) newLine=1;
          else if(!newParagraph){
            if (DEBUG) printf("      paragraph end!");
            write(output_fd,"\n\n",2);
            bytes_written=0;
            newLine=0;
            newParagraph=1;
          }
        }
        continue;
      }
      else{
        if (DEBUG) printf("\nbuilding word...\n");
        if(pos==word_length-1) word=realloc(word,sizeof(char)*(word_length*=2));
        word[pos++]=buff[i];
        newLine=0;
        newParagraph=0;
      }
  	}

    //if(isspace(buff[buff_length-1])) bytes_written+=write(output_fd," ",1);
    if(DEBUG) printf("\nbytes_read: %d\n", bytes_read);
    if(DEBUG) printf("bytes_written: %d\n", bytes_written);
    free(buff);
    buff = malloc(sizeof(char) * buff_length);
  }

  free(buff);
  free(word);
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
