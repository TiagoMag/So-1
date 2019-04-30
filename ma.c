#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct artigo{
 int codigo;
 float preco;
};

char* arr_token[4];

//i <nome> <preço> 
int insere(char** argv){
 int fd1=open("artigos.bin",O_CREAT|O_RDWR,0666);
 int fd2=open("strings.txt",O_CREAT|O_RDWR,0666);
 if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
 struct artigo Art;
 Art.codigo=lseek(fd2,0,SEEK_END);//determina o codigo do produto
 Art.preco=atof(argv[2]);
 char* BUFFER=strdup(argv[1]);
 int BufferSize=strlen(BUFFER)*sizeof(char);
 write(fd2,BUFFER,BufferSize); //escreve no strings o nome do produto
 lseek(fd1,0,SEEK_END);
 write(fd1,&Art,sizeof(struct artigo));//escreve o codigo e preco no artigos
 char *Buffer2=malloc(sizeof(char));
 char *newline=strdup("\n");
 BufferSize=strlen(newline)*sizeof(char);
 sprintf(Buffer2,"%d",Art.codigo);
 write(1,Buffer2,strlen(Buffer2)); // escreve no terminal o codigo do produto inserido
 write(1,newline,BufferSize);
 if (close (fd1)<0) write(1,"ERRO\n",5);
 if (close (fd2)<0) write(1,"ERRO\n",5);
 return 0;
}
//n <código> <novo nome>
 int update(char **argv){
 int fd1=open("artigos.bin",O_CREAT|O_RDWR,0666);
 int fd2=open("strings.txt",O_CREAT|O_RDWR,0666);
 if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
 int codigo;
 int BufferSize;
 char* BUFFER;
 ssize_t n;
 ssize_t counter=0;
 float preco;
 struct artigo aux;
 lseek(fd1,0,SEEK_SET);
 codigo=atoi(argv[1]);
 BUFFER=strdup(argv[2]);
 BufferSize=strlen(BUFFER)*sizeof(char);
 n=read(fd1,&aux,sizeof(struct artigo));
 while(n>0){ //procura onde se encontra o codigo no artigos
  counter+=n;
  if (aux.codigo==codigo) {preco=aux.preco;break;}
  n=read(fd1,&aux,sizeof(struct artigo));
  if (n==0) {puts("erro");return 1;}
 }
 lseek(fd2,0,SEEK_END);
 write(fd2,BUFFER,BufferSize);//escreve o novo nome no strings
 lseek(fd1,counter-sizeof(struct artigo),SEEK_SET);//posiciona o cursor onde se encontra o codigo do produto no ficheiro artigos
 codigo=lseek(fd2,0,SEEK_END)-1 ;//atualiza o codigo do produto
 aux.codigo=codigo;
 aux.preco=preco;
 write(fd1,&aux,sizeof(struct artigo));//altera o codigo antigo para o novo codigo
 if (close (fd1)<0) write(1,"ERRO\n",5);
 if (close (fd2)<0) write(1,"ERRO\n",5);
 return 0;
}

//p <código> <novo preço>
int alterapreco(char** argv){
 int fd1=open("artigos.bin",O_CREAT|O_RDWR,0666);
 if(fd1<0) write(1,"ERRO\n",5);
 int codigo;
 float preco;
 struct artigo aux;
 ssize_t n=0;
 ssize_t counter=0;
 preco=atof(argv[2]);
 codigo=atoi(argv[1]);
 n=read(fd1,&aux,sizeof(struct artigo));
 while(n>0){ //procura onde se encontra o codigo no artigos
  counter+=n;
  if (aux.codigo==codigo) {break;}
  n=read(fd1,&aux,sizeof(struct artigo));
  if (n==0) {puts("erro");return 1;}
 }
 lseek(fd1,counter-sizeof(struct artigo),SEEK_SET);//posiciona o cursor onde se encontra o codigo do produto no ficheiro artigos
 aux.preco=preco;
 aux.codigo=codigo;
 write(fd1,&aux,sizeof(struct artigo));
 if (close (fd1)<0) write(1,"ERRO\n",5);
 return 0;
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

int main(int argc,char** argv){
 char* token;
 char buffer [100];
 while(read(0,buffer,100)){
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
    alterapreco(arr_token);
    break;
  
   default:
   write(1,"ERRO\n",5);
  }
 }
}
 //fazer valida comandos
 return 0;
}
