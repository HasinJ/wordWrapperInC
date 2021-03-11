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


void writeStored(int output_fd, int* bytes_written, char* buffer, int start, int end) {
  if (DEBUG) printf(" width exceeded\n");
  write(output_fd,"\n",1);
  bytes_written=0;
  bytes_written+=write(1,&buffer[++start],--end);
}

void writeRegular(int output_fd, int* bytes_written, char* buff, int start, int end) {
  *bytes_written+=write(output_fd,&buff[start],end);
}

void output(int* bytes_written, char* buff, int start, int end, unsigned width, int output_fd){
  int length=end-start;
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
  int word_length = 6;
  char* buff = malloc(sizeof(char) * buff_length);
  char* word = calloc(word_length,sizeof(char));
  int bytes_read=0, bytes_written=0, wordStart=0, stored=0, pos=0;

  while ((bytes_read = read(input_fd, buff, buff_length)) > 0) {
    if (DEBUG) printBuffer(buff,buff_length);
  	for (i = 0; i < bytes_read; ++i) {
      if(isspace(buff[i])){
        if(!stored) {
          if (bytes_written+(i-wordStart)>width) writeStored(output_fd,&bytes_written,buff,wordStart,i);
          else writeRegular(output_fd,&bytes_written, buff, wordStart, i);
        }
        else {
          if(DEBUG) printf("printing what's in store...\n");
          if (bytes_written+(word_length+(i-wordStart))>width) writeStored(output_fd,&bytes_written,word,0,word_length);
          else writeRegular(output_fd,&bytes_written, word, 0, word_length);

          output(&bytes_written,buff,wordStart,i,width,output_fd);
          free(word);
          word = calloc(word_length,sizeof(char));
          stored=0;
          pos=0;
        }
        wordStart=i;
        continue;
      }
      //bytes_written=write(1,&buff[i],1);
  	}
    if(wordStart!=bytes_read-1 && wordStart!=0) storeWord(word,buff,wordStart,buff_length,&stored,&pos,bytes_read,&word_length);
    else if(!wordStart) {
      if(stored) {
        if(DEBUG) printf("printing what's in store2...\n");
        if (bytes_written+(word_length+(i-wordStart))>width) writeStored(output_fd,&bytes_written,buff,wordStart,i);
        else writeRegular(output_fd,&bytes_written, word, 0, word_length);
      }
      output(&bytes_written,buff,wordStart,i,width,output_fd);
      free(word);
      word = calloc(word_length,sizeof(char));
      stored=0;
      pos=0;
    }

    wordStart=0;
    //if(isspace(buff[buff_length-1])) bytes_written+=write(output_fd," ",1);
    if(DEBUG) printf("\nbytes_read: %d\n", bytes_read);
    if(DEBUG) printf("bytes_written: %d\n", bytes_written);
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
