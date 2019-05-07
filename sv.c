#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h> 
#include <signal.h>
#include <errno.h>

#define PIPE_BUF 1024

char* arr_token[3];

struct venda{
 int codigo;
 int quant;
 float ptotal;
};

struct artigo{
 int codigo;
 float preco;
};

struct stock{
 int codigo;
 int quant;
};

struct cliente{
int pid;
char buffer [50];
};


int read_pipe(int* fd,char* path,void* buf,int bufsize){
 int rd=read(*fd,buf,bufsize);
 if (rd<=0){
  close(*fd);
  *fd = open(path,O_RDONLY,066);
  rd=read(*fd,buf,bufsize);
 }
 return rd;
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

//<código_numérico> <quantidade> --> actualiza stock e mostra novo stock
int updatestock (char* arr_token[],int pid){
 struct stock aux;
 int found=0;
 int n=0;
 int fd1=open("stocks.txt",O_CREAT|O_RDWR,0666);
 if(fd1<0) write(1,"ERRO\n",5);
 int codigo=atoi(arr_token[0]);
 int quant=atoi(arr_token[1]);
 int counter=0;
 n=read(fd1,&aux,sizeof(struct stock));
 while(n>0){ //procura onde se existe o artigo no stocks
  counter+=n;
  if (aux.codigo==codigo) {
   found=1;
   if (quant>0){aux.quant=aux.quant+quant;}
   else if(quant<0) {aux.quant=aux.quant+quant;}
   if(aux.quant<0) {aux.quant=0;}
   lseek(fd1,counter-sizeof(struct stock),SEEK_SET);
   write(fd1,&aux,sizeof(struct stock));  
   break;  
  }
  n=read(fd1,&aux,sizeof(struct stock));
 }
 if (found==0) {
 aux.codigo=codigo;
 aux.quant=quant;
 if(quant<0) {aux.quant=aux.quant-quant;}
 if(aux.quant<0) {aux.quant=0;}
 write(fd1,&aux,sizeof(struct stock));
 }
 if (close (fd1)<0) write(1,"ERRO\n",5);
 return aux.quant;
}

float procurapreco (struct artigo *aux1,int fd2,int codigo){
 int n;
 float preco;
 n=read(fd2,aux1,sizeof(struct artigo));
 while(n>0){ //procura onde se encontra o codigo no artigos
  if (aux1->codigo==codigo) {preco=aux1->preco;break;}
  n=read(fd2,aux1,sizeof(struct artigo));
  if (n==0) {puts("erro");return -1;}
 }
 return preco;
}

int addVenda (char* arrtoken[]){
 struct venda *aux=malloc(sizeof(struct venda));
 struct artigo *aux1=malloc(sizeof(struct artigo)); 
 float preco;
 int fd1=open("vendas.txt",O_CREAT|O_RDWR,0666);
 int fd2=open("artigos.bin",O_CREAT|O_RDWR,0666);
 if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
 int codigo=atoi(arr_token[0]);
 int quant=atoi(arr_token[1]);
 aux->codigo=codigo;
 aux->quant=quant;
 preco=procurapreco(aux1,fd2,codigo);
 aux->ptotal=preco*(float)aux->quant;
  char buff[1024];
  sprintf(buff,"%d %d %f\n ",codigo,quant,aux->ptotal);
  puts(buff);
 if (quant<0){
  lseek(fd1,0,SEEK_END);
  write(fd1,&buff,strlen(buff));
 }
  if (close (fd1)<0) write(1,"ERRO\n",5);
  if (close (fd2)<0) write(1,"ERRO\n",5);
 return 0;
}

int main(int argc,char** argv){
  struct cliente aux2;
  char* token;
  int d;
  int fifo;
  int fdfifo;
  if (mkfifo("fifo",0666 )==-1); //write(1,"ERRO\n",5);
  fifo=open("fifo",O_RDONLY);
  /* Let's find out about broken client pipe via failed write() */

  if (signal(SIGPIPE,SIG_IGN) == SIG_ERR) printf("Erro: %s\n", strerror(errno));

  while(1){
    //get message
    read_pipe(&fifo,"fifo",&aux2,sizeof(struct cliente));
    //read(fifo,&aux2,sizeof(struct cliente));
    token=strtok(aux2.buffer,"\n");
    dividetoken(token,arr_token);
    if (arr_token[2]==NULL && strcmp(arr_token[0],"^C")!=0){
      addVenda(arr_token);
      //if (!addVenda(arr_token)) 
      d=updatestock(arr_token,aux2.pid);
      char buff[PIPE_BUF];
      sprintf(buff,"%s%d","fifo",aux2.pid);
 
      //send message
      fdfifo=open(buff,O_WRONLY);
      write(fdfifo,&d,sizeof(d));
      close(fdfifo);
    }
  }
// close(fifo);
// close(fdfifo);
 return 0;
}
