#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#define PIPE_BUF 1024

struct cliente{
int pid;
char buffer [50];
};

struct stock{
 int codigo;
 int quant;
};

int main (int argc,char** argv){ 
 struct cliente aux;
 struct stock aux2;
 char Buffer2[100];
 char buff[PIPE_BUF];
 sprintf(buff,"%s%d","fifo",(int)getpid());
 mkfifo(buff,0666);
 char buffer[50];
 int m,d,n;
 aux.pid=(int)getpid();
 while(read(0,buffer,50)){
  int fifo=open("fifo", O_WRONLY);
  strcpy(aux.buffer,buffer);
  write(fifo,&aux,sizeof(struct cliente));
  int clientfd=open(buff,O_RDONLY);
  read(clientfd,&d,sizeof(d));
  snprintf(Buffer2,sizeof(int),"%d",d);
  write(1,Buffer2,strlen(Buffer2));
  char *newline=strdup("\n");
  write(1,newline,2);
  close(fifo);

 }
 //close(fifo);
 return 0;
}