#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "global.h"


int dividetoken2(char* str,char* arr_token[]){
int j=0;
char* token;
token = strtok (str," ");
for(int i=1;i<3;i++)
arr_token[i]=NULL;
    while (token!=NULL){
    arr_token[j]=strdup(token);
    token=strtok(NULL," ");
    j++;
  }
return j;
}

int dividetoken(char* str,char* arr_token[]){
 int j=0;
 char* token;
 token = strtok (str," ");
 while (token != NULL){
  arr_token[j]=strdup(token);
  token = strtok (NULL, " ");
  j++;
 }
 return j;
}

ssize_t readln (int fd, void *buf, size_t nbyte) {
    nbyte--;
    char *cbuf = (char*) buf;
    int i;
    int rd=1;
    off_t foundnl=0;

    rd = read (fd, cbuf, nbyte);
    for (i=0;i<rd;i++)
        if (cbuf[i]=='\n'){
            foundnl=1;
            break;
        }
    cbuf[i]=0;
    lseek(fd,(i-rd)+foundnl,SEEK_CUR);

    return (foundnl == 1 ? i : -i);
}

ssize_t readln2 (int fd, void *buf, size_t nbyte) {
    nbyte--;
    char *cbuf = (char*) buf;
    int i;
    int rd=1;
    off_t foundnl=0;

    rd = read (fd, cbuf, nbyte);
    for (i=0;i<rd;i++)
        if (cbuf[i]=='/'){
            foundnl=1;
            break;
        }
    cbuf[i]=0;
    lseek(fd,(i-rd)+foundnl,SEEK_CUR);

    return (foundnl == 1 ? i : -i);
}

