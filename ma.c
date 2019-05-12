#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include "global.h"

#define Campos_Token 4

char* arr_token[Campos_Token];

//i <nome> <preço> 
int insere(char** argv){
  
  //if (!validainsere(argv)) return -1; //input invalido
  
  int fd1=open("artigos.txt",O_CREAT|O_RDWR,0666);
  int fd2=open("strings.txt",O_CREAT|O_RDWR,0666);

  if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
  
  struct artigo art;
  art.codigo=lseek(fd2,0,SEEK_END)+1;//determina o codigo do produto
  art.preco=atof(argv[2]);
  
  char* BUFFER=strcat(strdup(argv[1]),"/");
  int BufferSize=strlen(BUFFER)*sizeof(char);
  write(fd2,BUFFER,BufferSize); //escreve no strings o nome do produto
  
  lseek(fd1,0,SEEK_END);
  write(fd1,&art,sizeof(struct artigo));//escreve o codigo e preco no artigos

  char *Buffer2=malloc(sizeof(char));
  char *newline=strdup("\n");
  BufferSize=strlen(newline)*sizeof(char);
  sprintf(Buffer2,"%d",art.codigo);
  write(1,Buffer2,strlen(Buffer2)); // escreve no terminal o codigo do produto inserido
  write(1,newline,BufferSize);
 
 struct lixo lixo;
 int fd3=open("lixo.txt",O_CREAT|O_RDWR,0666);
lseek(fd3,0,SEEK_SET);
 read(fd3,&lixo,sizeof(struct lixo));
  lixo.total=lseek(fd2,0,SEEK_END);
 lseek(fd3,0,SEEK_SET);
 write(fd3,&lixo,sizeof(struct lixo));

 close(fd3);
  
  if (close (fd1)<0) write(1,"ERRO\n",5);
  if (close (fd2)<0) write(1,"ERRO\n",5);
  
  return 0;
}

//n <código> <novo nome>
 int update(char **argv){
 
 int fd1=open("artigos.txt",O_CREAT|O_RDWR,0666);
 int fd2=open("strings.txt",O_CREAT|O_RDWR,0666);
 
 if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
 
 int codigo;
 int BufferSize;
 char* BUFFER;
 ssize_t n=0;
 ssize_t counter=0;
 float preco;
 struct artigo art;
 
 lseek(fd1,0,SEEK_SET);
 
 codigo=atoi(argv[1]);
 BUFFER=strcat(strdup(argv[2]),"/");
 BufferSize=strlen(BUFFER)*sizeof(char);

 while((n=read(fd1,&art,sizeof(struct artigo)))>0){ //procura onde se encontra o codigo no artigos
  counter+=n;
  if (art.codigo==codigo) {preco=art.preco; break;}
  if (n==0) {puts("erro");return -1;} //não existe o artigo
 }
 
 lseek(fd2,0,SEEK_END);
 codigo=lseek(fd2,0,SEEK_END)+1 ;//atualiza o codigo do produto
 write(fd2,BUFFER,BufferSize);//escreve o novo nome no strings
 
 lseek(fd1,counter-sizeof(struct artigo),SEEK_SET);//posiciona o cursor onde se encontra o codigo do produto no ficheiro artigos
 
 
 art.codigo=codigo;
 art.preco=preco;
 
 
 lseek(fd2,atoi(argv[1])-1,SEEK_SET);
 
 
 char buffer[1024];


BufferSize= readln2(fd2,buffer,1024)+1;
 
 





 write(fd1,&art,sizeof(struct artigo));//altera o codigo antigo para o novo codigo
 struct lixo lixo;
 int fd3=open("lixo.txt",O_CREAT|O_RDWR,0666);
 lseek(fd3,0,SEEK_SET);
 read(fd3,&lixo,sizeof(struct lixo));
 lixo.lixo=lixo.lixo+BufferSize;
 lixo.total=lseek(fd2,0,SEEK_END);
 lseek(fd3,0,SEEK_SET);
 write(fd3,&lixo,sizeof(struct lixo));

 close(fd3);
 if (close (fd1)<0) write(1,"ERRO\n",5);
 if (close (fd2)<0) write(1,"ERRO\n",5);
 
 return 0;
}

//p <código> <novo preço>
float alterapreco(char** argv){
 
 int fd1=open("artigos.txt",O_CREAT|O_RDWR,0666);
 
 if(fd1<0) write(1,"ERRO\n",5);
 
 int codigo;
 float preco;
 struct artigo art;
 ssize_t n=0;
 ssize_t counter=0;
 
 preco=atof(argv[2]);
 codigo=atoi(argv[1]);
 
 while((n=read(fd1,&art,sizeof(struct artigo)))>0){ //procura onde se encontra o codigo no artigos
  counter+=n;
  if (art.codigo==codigo) break;
  if (n==0) {puts("erro");return -1;} //não existe o artigo
 }
 
 lseek(fd1,counter-sizeof(struct artigo),SEEK_SET);//posiciona o cursor onde se encontra o codigo do produto no ficheiro artigos
 art.preco=preco;
 art.codigo=codigo;
 write(fd1,&art,sizeof(struct artigo));
 
 if (close (fd1)<0) write(1,"ERRO\n",5);
 
 return preco;
}

int main(int argc,char** argv){
 
 struct mudaPreco mp;
 char buffer [1024];
 int fd_fifo;
 float newpreco;
 char* token;
 
 mkfifo("precochange",0666);
 
 while(readln(0,buffer,1024)){
  
  token=strtok(buffer,"\n");
  dividetoken(token,arr_token);
  
  if (arr_token[3]!=NULL||arr_token[2]==NULL||arr_token[0]==NULL||arr_token[1]==NULL) 
   
   {puts("Comando invalido");}
 
  else{
   
   switch(buffer[0])
   
   {
    case ('i'):
    insere(arr_token);
    break;
  
    case('n'):
    update(arr_token);
    break;
  
    case('p'):
    newpreco=alterapreco(arr_token);
    fd_fifo=open("precochange", O_WRONLY);
    mp.flag=1;
    mp.codigo=atoi(arr_token[1]);
    mp.preco=newpreco;
    write(fd_fifo,&mp,sizeof(struct mudaPreco));
    close(fd_fifo);
    break;
  
   default:
   write(1,"ERRO\n",5);
  }
 }
}

 return 0;
}
