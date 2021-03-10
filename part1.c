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

void storeWord(char* word, char* buff, int wordStart, int end) {
  size_t i,j=0;
  for (i = wordStart; i < end; i++) {
    word[j]=buff[i];
    j++;
  }
}

int main(int argc, char const *argv[]) {

  if (argc>2 || argc==1) {
    printf("Please enter just the width for now.\n");
    return EXIT_SUCCESS;
  }

  int file = open("testcase.txt",O_RDONLY);
  int length = 4;
  char* buff = malloc(sizeof(char) * length);
  char* word = malloc(sizeof(char) * length);
  int bytes_read=0, bytes_written=0, width=atoi(argv[1]), wordStart=0;

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
 store based on word wordStart

 spec[tacu]
 store based on word start but need to realloc

 [lar ]
 

*/


  size_t i=0;
  while ((bytes_read = read(file, buff, length)) > 0) {
  	for (i = 0; i < bytes_read; ++i) {
      if(isspace(buff[i])){
        //word[length]='\0';
        bytes_written=write(1,&word[0],strlen(word));
        wordStart=i;
      }
      //bytes_written=write(1,&buff[i],1);
  	}
    storeWord(word,buff,wordStart,length);


    printf("\nbytes_read: %d\n", bytes_read);
  }
  free(buff);
  close(file);
  printf("\nbytes_written: %d\n", bytes_written);
  printf("width: %d\n", width);

  return 0;
}
