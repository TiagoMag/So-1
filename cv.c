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

//retornar a quantidade em stock e o preco de um artigo




int main (int argc,char** argv){ 
 struct cliente aux;
 char Buffer2[100]; 
 char buff[PIPE_BUF];
    sprintf(buff,"%s%d","fifo",(int)getpid()); //passa um int para char
    mkfifo(buff,0666);
char buffer[50]; //buffer que depois vai escrever no buffer do cliente
int d;
aux.pid=(int)getpid();
     while(read(0,buffer,50)){
int fd_fifo=open("fifo", O_WRONLY);
    strcpy(aux.buffer,buffer); // escrever no buffer do cliente o do buffer anterior
    write(fd_fifo,&aux,sizeof(struct cliente));
    int clientfd=open(buff,O_RDONLY);
    read(clientfd,&d,sizeof(d));
    snprintf(Buffer2,sizeof(int),"%d",d); //int to string
    write(1,Buffer2,strlen(Buffer2)); //write no  sdtdout
    char *newline=strdup("\n");
    write(1,newline,2);
    close(fd_fifo);
}
//retornar a quantidade em stock e o preco de um artigo
//int fd_stock = open ("STOCKS.txt",O_RDONLY);
//float procurapreco (struct artigo *aux1,int fd2,int codigo)




//close(fifo);
 return 0;
}