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


int main(int argc, char const *argv[]) {

  if (argc>2 || argc==1) {
    printf("Please enter just the width for now.\n");
    return EXIT_SUCCESS;
  }

  int file = open("testcase.txt",O_RDONLY);
  int buff_length = 4;
  int word_length = 6;
  char* buff = malloc(sizeof(char) * buff_length);
  char* word = calloc(word_length,sizeof(char));

  int bytes_read=0, bytes_written=0, width=atoi(argv[1]), wordStart=0, stored=0,pos=0;

/*
  repetitive:
  - store based on word start
  - reset word start at the end
  - for every write, word gets emptied out and write gets issued,
  - not always storing at the end, if length and wordstart are equal then no store


 [hell]
 store based on word start, reached end with no space, reset wordstart

 hell[o wo]
      ^ write here, issue word start
          ^ store based on word start, ends with no space, reset word start

 wo[rld!]
 store based on word start, must realloc for more space, reached end with no space

 world![ my ]
       ^ write here, issue word start
          ^ write here, issue word start again
          ^ reset word start, store nothing because length and wordstart are equal

 [name]
 store based on word start, reached end with no space, reset wordStart

 name[ is ]
      ^ write here, issue word start
         ^ write again, issue word wordStart
         ^ reset word start, store nothing because length and wordstart are equal

 [spec]
 just write

 [tacu]
 just write

 [lar ]
 just write

*/


  size_t i=0;
  while ((bytes_read = read(file, buff, buff_length)) > 0) {
    if (DEBUG) printBuffer(buff,buff_length);
  	for (i = 0; i < bytes_read; ++i) {
      if(isspace(buff[i])){
        if(!stored) bytes_written=write(1,&buff[wordStart],i);
        else {
          if(DEBUG) printf("printing what's in store...\n");
          bytes_written+=write(1,&word[0],word_length);
          bytes_written+=write(1,&buff[wordStart],i);
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
      if(stored) bytes_written+=write(1,&word[0],word_length);
      bytes_written+=write(1,&buff[wordStart],i);
      free(word);
      word = calloc(word_length,sizeof(char));
      stored=0;
      pos=0;
    }

    wordStart=0;

    if(DEBUG) printf("\nbytes_read: %d\n", bytes_read);
  }
  free(buff);
  free(word);
  close(file);
  if(DEBUG) printf("\nbytes_written: %d\n", bytes_written);
  if(DEBUG) printf("width: %d\n", width);

  return 0;
}
