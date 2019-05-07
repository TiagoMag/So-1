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
  
  if (rd<=0)  {
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

int procuracodigo (struct artigo *aux1,int fd2,int codigo){
int n;
n=read(fd2,aux1,sizeof(struct artigo));
    while(n>0){ //procura onde se encontra o codigo no artigos
      if (aux1->codigo==codigo) {break;}
    n=read(fd2,aux1,sizeof(struct artigo));
    if (n==0) {return -1;}
    }
return 0;
}

//<código_numérico> <quantidade> --> actualiza stock e mostra novo stock
int updatestock (char* arr_token[],int pid){
struct stock aux;
int found=0,n=0;
struct artigo *aux1=malloc(sizeof(struct artigo));
int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
int fd1=open("stocks.txt",O_CREAT|O_RDWR,0666);
  
  if(fd1<0) perror("ERRO");
 int codigo=atoi(arr_token[0]);
 int quant=atoi(arr_token[1]);
 int counter=0;
 n=read(fd1,&aux,sizeof(struct stock));
 
 if (procuracodigo(aux1,fd2,codigo)<0) return -1;
 

 while(n>0){ //procura onde e se existe o artigo no stocks
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

   if (close (fd1)<0) perror("ERRO");
   if (close (fd2)<0) perror("ERRO");
 
return aux.quant;
}
/*
int seestock(char* arr_token[],int pid){
struct stock aux;
struct artigo *aux1=malloc(sizeof(struct artigo));
int found=0,n=0,value,i=0;
int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
int fd1=open("stocks.txt",O_CREAT|O_RDWR,0666);
       if(fd1<0) perror("ERRO");
    int codigo=atoi(arr_token[0]);
    int quant=atoi(arr_token[1]);
    int counter=0;
    n=read(fd1,&aux,sizeof(struct stock));
      if (procuracodigo(aux1,fd2,codigo)<0) return -1;
        else {
        while(n>0){ //procura onde se existe o artigo no stocks
        counter+=n;
      if (aux.codigo==codigo) {
      found=1;
         
        }
        
    }
}

    counter=0;
    i=read(fd2,&aux1,sizeof(struct artigos));
      if (procuracodigo(aux1,fd2,codigo)<0)return -1;
      else{
      while(n>0){
        counter+=n;
      if (aux.codigo==codigo) {
        found=1;
      value=procurapreco(artigo *aux1,fd2,codigo){
             }
          }  
      }   

return 0;
}
*/

int showstock (struct stock *aux,int fd1, int codigo){
int found=0,n=0,stock;; 
//char *buff_stok;
n=read(fd1,aux,sizeof(struct stock));
     while (n>0){
      
      if (aux->codigo==codigo){
      found=1;
        }       
      if (found==1) {
      //sprintf(*buff_stok,"%d",show.quant);
      stock=aux->quant; break;
      }
      
      else {
      perror("Stock inexistente");
    }
      
      if (n==0) {
      perror("error"); return 1;}
  }
  
return stock;
}

float procurapreco (struct artigo *aux1,int fd2,int codigo){
 int n;
 float preco;
 n=read(fd2,aux1,sizeof(struct artigo));
 while(n>0){ //procura onde se encontra o codigo no artigos
  if (aux1->codigo==codigo) {preco=aux1->preco;break;}
  n=read(fd2,aux1,sizeof(struct artigo));
  if (n==0) {return -1;}
 }
 return preco;
}

int addVenda (char* arrtoken[]){
struct venda *aux=malloc(sizeof(struct venda));
struct artigo *aux1=malloc(sizeof(struct artigo)); 

float preco;
int fd1=open("vendas.txt",O_CREAT|O_RDWR,0666);
int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);

    if(fd1<0 || fd2<0) perror("ERRO");

      int codigo=atoi(arr_token[0]);
      int quant=atoi(arr_token[1]);
      aux->codigo=codigo;
      aux->quant=quant;
      preco=procurapreco(aux1,fd2,codigo);

    if (preco<0) return -1;

      aux->ptotal=preco*(float)aux->quant;
      char buff[1024];
      sprintf(buff,"%d %d %f\n ",codigo,quant,aux->ptotal);
      puts(buff);

    if (quant<0){

      lseek(fd1,0,SEEK_END);
      write(fd1,&buff,strlen(buff));
    }

    if (close (fd1)<0) perror("ERRO");;
    if (close (fd2)<0) perror("ERRO");;

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
      if (addVenda(arr_token)==-1) puts("Codigo not found");
      //if (!addVenda(arr_token)) 
      //d=updatestock(arr_token,aux2.pid);
      char buff[PIPE_BUF];
      sprintf(buff,"%s%d","fifo",aux2.pid);
      //if (updatestock(arr_token,aux2.pid)==-1) puts("Codigo not found"); 
      //send message
      if ((d=updatestock(arr_token,aux2.pid))>=0){
      fdfifo=open(buff,O_WRONLY);
      write(fdfifo,&d,sizeof(d));
      close(fdfifo);
    }else puts("Codigo no found");
    }
  }
// close(fifo);
// close(fdfifo);
 return 0;
}
