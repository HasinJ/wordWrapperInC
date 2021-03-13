#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<ctype.h>
#include<assert.h>
#include <sys/stat.h>


#ifndef DEBUG
#define DEBUG 0
#endif

static int status = EXIT_SUCCESS;

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
	
    if(isspace((unsigned char)buff[start])){
      if (DEBUG) printf("BOOM bug\n"); //no space exists
    }
  }

  if(*bytes_written!=0 && !lastWord) *bytes_written+=write(output_fd," ",1);
  *bytes_written+=write(output_fd,&buff[start],end);
  
}



void wrap(unsigned width, int input_fd, int output_fd){
  size_t i=0;
  int buff_length = 4;
  int word_length = buff_length;
  char* buff = malloc(sizeof(char) * buff_length);
  char* word = calloc(word_length,sizeof(char));
  int bytes_read=0, bytes_written=0,pos=0,lastWord=0,newLine=0,newParagraph=0;

  while ((bytes_read = read(input_fd, buff, buff_length)) > 0) {
    if (DEBUG) printBuffer(buff,buff_length); //paragraph(buff, i, output_fd, &bytes_written)

  	for (i = 0; i < bytes_read; ++i) {
      if(isspace((unsigned char) buff[i])) {
        if(pos) {
          if (DEBUG) printf("dumping word...\n");
          writeStored(&bytes_written,lastWord,word,0,pos,pos,width,output_fd);
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
	if (bytes_written > width) {
		if (DEBUG) printf("\n A word exceeded width\n");
		status = EXIT_FAILURE;
	}
	
    free(buff);
    buff = malloc(sizeof(char) * buff_length);
  }
    if (DEBUG) printf("final dump word...\n");
    writeStored(&bytes_written,1,word,0,pos,pos,width,output_fd); //clear stored word
	write(output_fd,"\n",1); //end with newline
	
  free(buff);
  free(word);
}//end wrap



int isdir(char *name) {

    struct stat data;
    int err = stat(name, &data);
    if (err) {
        perror(name);
        return 0;
    }
    if (S_ISDIR(data.st_mode)) {
        return 1;
    }
    if (S_ISREG(data.st_mode)) {
        return 2;
    }


    return 0;
}

int ww(int wid, char *input) {

    assert(wid>0);

	if (input[0] == '\n') {  //strcmp(input,"")
        
		wrap (wid, 0, 1);
	}
    
	else if (isdir(input)==1) {
        DIR *dirp = opendir(input);
        struct dirent *de;
        char* wr = "wrap.";
		chdir(input);

        while ((de = readdir(dirp))) {
            
			
			if (isdir(de->d_name)==2 && strncmp(de->d_name,".",1)!=0 && strncmp(de->d_name,wr,1)!=0 ) {
            int file = open(de->d_name, O_RDONLY);
            char* cur = calloc(strlen(de->d_name)+5,sizeof(char));
            strcpy(cur,wr);
			strcat(cur,de->d_name);
			
            int out = open(cur, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
			if(DEBUG) printf("\ncurrent file: %s\n", de->d_name);			
            wrap(wid,file,out);
			if(DEBUG) printf("\nwrap failed, files: %s\n%s\n", de->d_name, cur);
            free(cur);
			 
            }
		}
		
		closedir(dirp);
	}
	
	else  {  //(isdir(input)==2)
        int file = open(input, O_RDONLY);
        if (file == -1){
			perror("Could not open file: ");
			exit(EXIT_FAILURE);
		}
		wrap(wid, file, 1);
		close(file);
		//int *p =   if (!p) return 1;
    }

	return 0;
} //end ww

int main(int argc, char *argv[]) {

	if (argc == 2) {
		if (DEBUG)printf("\nAbout to attempt\n");
		//pipe possible?
		ww(atoi(argv[1]),"\n");
		if(DEBUG) printf("failed");
	}
	
	if (argc == 3) {
		if (DEBUG) printf("\nAbout to attempt\n");
		ww(atoi(argv[1]),argv[2]);
		if(DEBUG) printf("\ninputs: %s\n%s\n", argv[1], argv[2]);
	}

	if (DEBUG) printf("\nMade it to end\n");
 
  exit(status);
}