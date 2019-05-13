#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include "global.h"

#define PIPE_BUF 1024



void ctrlc_handler (int signum){
 
 int size=sizeof(int)+4*sizeof(char);
 char buff[size];
 sprintf(buff,"%s%d","tmp/fifo",(int)getpid()); 
 unlink(buff);
 exit(0);

}

int main (int argc,char** argv){
 
 struct message_client c;
 struct request_client r;

 char BUFF[1024],buff[PIPE_BUF],buffer[1024]; 
 
 signal(SIGINT,ctrlc_handler);

 sprintf(buff,"%s%d","tmp/fifo",(int)getpid());//criar nome para o fifo do cliente
 if(mkfifo(buff,0666)==-1 && (errno != EEXIST)) 
 perror("Problema a criar o fifo");
 
 int client_fifo, server_fifo;
 
 c.pid=(int)getpid();
 
 if((server_fifo=open("tmp/fifo", O_WRONLY))==-1)
  {perror("Criar fifo OLE");}
  
  while(readln(0,buffer,50)){
       
    strcpy(c.buffer,buffer); 
       
    //escrever mensagem no servidor
    write(server_fifo,&c,sizeof(struct message_client));

    //ler resposta servidor
    client_fifo=open(buff,O_RDONLY);
    read(client_fifo,&r,sizeof(struct request_client));
    close(client_fifo);
    
    if(r.stock>=0){
    
    snprintf(BUFF,sizeof(int),"%d",r.stock); //int to string
    write(1,BUFF,strlen(BUFF)); //write no  sdtdout
    
    }
    
    if (r.preco>=0){
     
     char *newline=strdup("\n");
     write(1,newline,2);

     char buf[16];
     sprintf(buf, "%0.2lf", r.preco);  //int to string
     write(1,buf,strlen(buf)); //write no  sdtdout 
        
    }
    
    char *newline=strdup("\n");
    write(1,newline,2);
    
    }
    
    close(server_fifo);
 
 return 0;

}