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
#include <wait.h>
#include "global.h"

#define PIPE_BUF 1024
#define Campos_Token 3
#define TOP 100000

char* arr_token[Campos_Token];
struct top topvendas[TOP];


int procuracodigo (struct artigo *aux,int fd,int codigo){
 
 int n;
 n=read(fd,aux,sizeof(struct artigo));
 
 while(n>0){ //procura onde se encontra o codigo no artigos
  if (aux->codigo==codigo) {break;}
   n=read(fd,aux,sizeof(struct artigo));
   if (n==0) {return -1;}
 }

 return 0;

}

//<código_numérico> <quantidade> --> actualiza stock e mostra novo stock
int updatestock (char* arr_token[],int pid){

 struct stock stock;
 struct artigo *art=malloc(sizeof(struct artigo));
 
 int found,n;
 found=n=0;

 int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
 int fd1=open("stocks.txt",O_CREAT|O_RDWR,0666);
 if(fd1<0) perror("ERRO");

 int codigo=atoi(arr_token[0]);
 int quant=atoi(arr_token[1]);
 int counter=0;
 
 if (procuracodigo(art,fd2,codigo)<0) return -1; //ver se codigo existe
 
 while((n=read(fd1,&stock,sizeof(struct stock)))>0){ //procura onde e se existe o artigo no stocks
  
  counter+=n;
  if (stock.codigo==codigo) {
   found=1;
   if (quant>0){stock.quant=stock.quant+quant;}
   else if(quant<0) {stock.quant=stock.quant+quant;}
   if(stock.quant<0) {stock.quant=0;}
   lseek(fd1,counter-sizeof(struct stock),SEEK_SET);
   write(fd1,&stock,sizeof(struct stock));  
   break;  
  }
 }

 if (found==0) { //se não exister o artigo nos stocks cria o seu stock
  
  stock.codigo=codigo;
  stock.quant=quant;
  if(quant<0) {stock.quant=stock.quant-quant;}
  if(stock.quant<0) {stock.quant=0;}
  write(fd1,&stock,sizeof(struct stock));
  
 }

 if (close (fd1)<0) perror("ERRO");
 if (close (fd2)<0) perror("ERRO");

 return stock.quant;

}

//--
int seestock(char* arr_token[]){
 
 struct stock stock;
 int n=0,value;
 int fd=open("stocks.txt",O_RDONLY);
 int codigo=atoi(arr_token[0]);

if(fd<0) perror("ERRO");  
  while((n=read(fd,&stock,sizeof(struct stock)))>0){ //procura onde se encontra o codigo no artigos
    if (stock.codigo==codigo) {value=stock.quant; break;}
      if (n==0) {
      return -1;
      }   
    
    }
 if(close(fd)<0)perror("ERRO");

 return value;

}

//--
float procurapreco_main (char* arr_token[]){

 struct artigo art;
 int n;
 float preco;
 int fd=open("artigos.txt",O_RDONLY);
 int codigo=atoi(arr_token[0]);
  
  if(fd<0) perror ("ERRO");
    while((n=read(fd,&art,sizeof(struct artigo)))>0){ //procura onde se encontra o codigo no artigos
      if (art.codigo==codigo){
      preco=art.preco;

      } 
      if (n==0) {
      return -1;
      }
  
    }

 if(close(fd)<0)perror("ERRO");
 
 return preco;

}
   

float procurapreco (struct artigo *aux,int fd,int codigo){

 int n;
 float preco;
 n=read(fd,aux,sizeof(struct artigo));

  while(n>0){ //procura onde se encontra o codigo no artigos
   if (aux->codigo==codigo) {preco=aux->preco;break;}
   n=read(fd,aux,sizeof(struct artigo));
   if (n==0) {return -1;}
  }

return preco;
}

int preenchetop(){
 
  pid_t pid;
 
  if ((pid=fork())==0){
    
    int fd1 = open ("gr.txt",O_RDONLY);
    int fd2 = open("top.txt",O_WRONLY | O_CREAT, 0666);

    dup2(fd1,0);
    dup2(fd2,1);

    execlp("sort","sort","-k","2","-nr",NULL);
    close(fd1);
    close(fd2);
  
  }else{

  wait(NULL); 
  
  int fd1=open("top.txt",O_RDWR|O_CREAT,0666);
  int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
  
  struct artigo *art=malloc(sizeof(struct venda));
 
  int n=1;
  char buffer[1024];
  char *token;
  
  for(int i=0;i<TOP;topvendas[i++].codigo=-1,topvendas[i++].preco=-1); //inicializa "cache"

  for (int i=0;i<TOP && n>0;i++){
   
   n=readln(fd1,buffer,1024);
   if (n==0) break;
   
   token=strtok(buffer,"\n");
   dividetoken2(token,arr_token);
   topvendas[i].codigo=atoi(arr_token[0]);
   topvendas[i].preco=procurapreco(art,fd2,topvendas[i].codigo);
 
  }
 
  close (fd1);
  close (fd2); 

 }

 return 0;

}


float procuratop(int codigo){

 float preco;
 int i;
 
 for(i=0;i<TOP;i++){
  if (topvendas[i].codigo==codigo)
  {preco=topvendas[i].preco;break;}
 }

 if (i==TOP) return -1; //Não se encontra no top o artigo
return preco;


}

int addVenda (char* arrtoken[]){
 
 struct venda *venda=malloc(sizeof(struct venda));
 struct artigo *art=malloc(sizeof(struct artigo)); 
 
 float preco;
 
 int fd1=open("vendas.txt",O_CREAT|O_RDWR,0666);
 int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
 
 if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
 
 int codigo=atoi(arr_token[0]);
 int quant=abs(atoi(arr_token[1]));
 
 venda->codigo=codigo;
 venda->quant=quant;
 
 preco=procuratop(codigo); //ve se preço está na cache
 
 if(preco==-1)
 preco=procurapreco(art,fd2,codigo); //vê se preço está em artigos
 
 if (preco<0) return -1; //se não está em artigos não existe
 
 venda->ptotal=abs(preco*(float)venda->quant);
 
 char buff[1024];
 sprintf(buff,"%d %d %f\n",codigo,quant,venda->ptotal); //cria linha venda
 
 
 lseek(fd1,0,SEEK_END);
 write(fd1,buff,strlen(buff));
 
 if (close (fd1)<0) write(1,"ERRO\n",5);
 if (close (fd2)<0) write(1,"ERRO\n",5);
 
 return 0;
}

int alterarray(float preco,int codigo ){

 for(int i=0;i<20;i++){
  if(topvendas[i].codigo==codigo) {topvendas[i].preco=preco; break;}
 }

 return 0;

}

//--
int main(int argc,char** argv){

 struct message_client mc;

 char* token;
 int server_fifo,client_fifo,fifo2;
 struct mudaPreco c;
 
 preenchetop();

  //criar fifo do servidor 
  if(mkfifo("tmp/fifo",0666)<0)
 perror("Problema a criar o fifo");
 server_fifo=open("tmp/fifo",O_RDONLY);
 
 fifo2=open("precochange", O_RDONLY|O_NONBLOCK);

 if (signal(SIGPIPE,SIG_IGN) == SIG_ERR) printf("Erro: %s\n", strerror(errno));
      
  while(1){
    
   
    read(fifo2,&c,sizeof(struct mudaPreco));
    if (c.flag==1) alterarray(c.preco,c.codigo);
   
    //close(fifo2);
    //read_pipe(&server_fifo,"tmp/fifo",&mc,sizeof(struct message_client));
    
    //lê mensagem do cliente 
    read(server_fifo,&mc,sizeof(struct message_client));
    
    token=strtok(mc.buffer,"\n");
    dividetoken2(token,arr_token);
    
    if(arr_token[0]!=NULL && arr_token[1]==NULL){ // ex 0->mostra preco e stock
    
    struct request_client r;
    char buff[1024];
    
    sprintf(buff,"%s%d","tmp/fifo",mc.pid);
    
    if((r.stock=seestock(arr_token))>=0){
    
    }

    if((r.preco=procurapreco_main(arr_token))>=0){   // se nao exister preco codigo nao existe
     
     client_fifo = open(buff,O_WRONLY);
     write(client_fifo,&r,sizeof(struct request_client));
     close(client_fifo);
    
    }
    else 
    puts("Codigo not available");
  
    }
    
    else if (arr_token[0]!=NULL && arr_token[1]!=NULL && arr_token[2]==NULL){
    
    if (addVenda(arr_token)==-1) puts("Codigo not found");
    
    struct request_client r;
   
    char buff[PIPE_BUF];
    sprintf(buff,"%s%d","tmp/fifo",mc.pid);
 
    if ((r.stock=updatestock(arr_token,mc.pid))>=0){
    
    r.preco=-1;   

    client_fifo=open(buff,O_WRONLY);
    write(client_fifo,&r,sizeof(struct request_client));
    close(client_fifo);


    }
    else puts("Codigo no found");                     
    } 
    
  }
 
 close(server_fifo);

return 0;
}
