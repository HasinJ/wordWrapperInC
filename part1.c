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

int name(/* arguments */) {
  int buff_length = 4;
  int word_length = 6;
  char* buff = malloc(sizeof(char) * buff_length);
  char* word = calloc(word_length,sizeof(char));

  int bytes_read=0, bytes_written=0, wordStart=0, stored=0, pos=0;
  unsigned width=atoi(argv[1]);




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
  }        stored=0;
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
}

int main(int argc, char const *argv[]) {

  if (argc>2 || argc==1) {
    printf("Please enter just the width for now.\n");
    return EXIT_SUCCESS;
  }

  int file = open("testcase.txt",O_RDONLY);

  free(buff);
  free(word);
  close(file);
  if(DEBUG) printf("\nbytes_written: %d\n", bytes_written);
  if(DEBUG) printf("width: %d\n", width);

  return 0;
}
