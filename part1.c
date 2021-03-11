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


int paragraph(char* buff, size_t* i, int output_fd, int* bytes_written) {
  if(buff[*i+1]==10 && buff[*i]==10) {
    write(output_fd,"\n",2);
    if (DEBUG) printf("\nparagraph end! ASCIIs: %d %d ", buff[*i], buff[*i+1]);
    *i+=2;
    *bytes_written=0;
    if (DEBUG) printf(" i: %ld\n", *i);
    return 1;
  }
  return 0;
}

int countSpaces(char* buff, int i) {
  return 1;
}

void storeWord(char* word, char* buff, int wordStart, int end, int* stored,int* pos, int bytes_read, int* word_length) {
  size_t i;
  if(isspace(buff[wordStart])) wordStart++;
  for (i = wordStart; i < end; i++) {
    word[*pos]=buff[i];
    (*pos)++;
  }
  *stored=1;
}

void printBuffer(char* buff, int buff_length){
  size_t i;
  for (i = 0; i < buff_length; i++) {
    printf("position: %ld letter: %c ASCII: %d\n", i, buff[i], buff[i]);
  }
}


void output(int* bytes_written, char* buff, int start, int end, int length, unsigned width, int output_fd){
  if(*bytes_written+length>width) {
    write(output_fd,"\n",1);
    *bytes_written=0;
    if(isspace(buff[start])){
      if (DEBUG) printf(" and starting is space\n");
      start++;
      end--;
    }
  }
  *bytes_written+=write(output_fd,&buff[start],end);
}


void wrap(unsigned width, int input_fd, int output_fd){
  size_t i=0;
  int buff_length = 4;
  int word_length = buff_length;
  char* buff = malloc(sizeof(char) * buff_length);
  char* word = calloc(word_length,sizeof(char));
  int bytes_read=0, bytes_written=0, wordStart=0, stored=0, pos=0;

  while ((bytes_read = read(input_fd, buff, buff_length)) > 0) {
    if (DEBUG) printBuffer(buff,buff_length);

  	for (i = 0; i < bytes_read; ++i) {
      if(isspace(buff[i])){


        if(stored){
          if(DEBUG) printf("printing what's in store...\n");
          output(&bytes_written, word, 0, pos, (pos)+(i-wordStart), width, output_fd);
          output(&bytes_written, buff, wordStart, i+1, 0, width, output_fd);

          free(word);
          word = calloc(word_length,sizeof(char));
          stored=0;
          pos=0;
        }


        else {
          if(DEBUG) printf("partial word...\n");
          if(pos+((buff_length-1)-wordStart)>=word_length-1) {
            if(DEBUG) printf("reallocating...\n");
            word=realloc(word,sizeof(char)*(word_length*=2));
          }
          storeWord(word,buff,wordStart,buff_length,&stored,&pos,bytes_read,&word_length);
        }

        wordStart=i;
        continue;
      }


      //bytes_written=write(1,&buff[i],1);
  	}

    if(wordStart!=bytes_read-1) {
      if(DEBUG) printf("\npartial word2...\n");
      if(pos+((buff_length-1)-wordStart)>=word_length-1) {
        if(DEBUG) printf("reallocating...\n");
        word=realloc(word,sizeof(char)*(word_length*=2));
      }
      storeWord(word,buff,wordStart,buff_length,&stored,&pos,bytes_read,&word_length);
    }

    wordStart=0;

    //if(isspace(buff[buff_length-1])) bytes_written+=write(output_fd," ",1);
    if(DEBUG) printf("\nbytes_read: %d\n", bytes_read);
    if(DEBUG) printf("bytes_written: %d\n", bytes_written);
    free(buff);
    buff = malloc(sizeof(char) * buff_length);
  }
  if(stored) {
    output(&bytes_written, word, 0, pos, (pos)+(i-wordStart), width, output_fd);
  }
  free(buff);
  free(word);
}



int main(int argc, char const *argv[]) {

  if (argc>2 || argc==1) {
    printf("Please enter just the width for now.\n");
    return EXIT_SUCCESS;
  }

  int fileDsc = open("testcase.txt",O_RDONLY);
  unsigned width=atoi(argv[1]);
  wrap(width,fileDsc,1);

  close(fileDsc);
  if(DEBUG) printf("width: %d\n", width);

  return 0;
}
