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


//efectuar vendas ou entrada em stock, especificando o código e quantidade 


//int vendstok(int argc, char* argv[]){

//!!

/*
int showstok (int argc, char* argv[]){
struct stock aux;
char BUFF[100], buff[PIPE_BUF]; 
sprintf(buff,"%s%d","fifo",(int)getpid());//passa um int para char
mkfifo(buff,0666);
char buffer[50];//buffer que depois vai escrever no buffer do cliente
int d, stockfd, fd_fifo;
  aux.pid=(int)getpid();  
  fd_fifo=open("fifo", O_WRONLY);
      while(readln(0,buffer,50)){// send message to server 
          strcpy(aux.,buffer); // escrever no buffer do cliente o do buffer anterior
          write(fd_fifo,&aux,sizeof(struct stock));
          // read the answer
          stockfd=open(buff,O_RDONLY);
          read(stockfd,&d,sizeof(d));
          //close(stockfd);
          snprintf(BUFF,sizeof(int),"%d",d); //int to string
          write(1,BUFF,strlen(BUFF)); //write no  sdtdout
          char *newline=strdup("\n");
          write(1,newline,2);
          //close(fd_fifo);
      close(stockfd);
         }
return 0;
}
*/

int main (int argc,char** argv){
struct cliente aux;
char BUFF[100],buff[PIPE_BUF]; 
sprintf(buff,"%s%d","fifo",(int)getpid());//passa um int para char
mkfifo(buff,0666);
char buffer[50];//buffer que depois vai escrever no buffer do cliente
int d,clientfd, fd_fifo;
aux.pid=(int)getpid();
fd_fifo=open("fifo", O_WRONLY);
    while(readln(0,buffer,50)){
        // send message to server 
        strcpy(aux.buffer,buffer); // escrever no buffer do cliente o do buffer anterior
        write(fd_fifo,&aux,sizeof(struct cliente));
        // read the answer
        clientfd=open(buff,O_RDONLY);
        read(clientfd,&d,sizeof(d));
        //close(clientfd);
        snprintf(BUFF,sizeof(int),"%d",d); //int to string
        write(1,BUFF,strlen(BUFF)); //write no  sdtdout
        char *newline=strdup("\n");
        write(1,newline,2);
        //close(fd_fifo);
    close(clientfd);
    }
return 0;
}