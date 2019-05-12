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


int crianovapos(struct artigo art){

 char buffer[1024];
 char BUFFER[1024];
 int codigo;
 int fd1=open("strings.txt",O_CREAT|O_RDWR,0666);
 lseek(fd1,art.codigo-1,SEEK_SET);
 read(fd1,buffer,1024);
 
 int i=0;

 int fd2=open("strings2.txt",O_CREAT|O_RDWR,0666);

 read(fd1,buffer,sizeof(char));

 
 while(buffer[i++]!='/')
 read(fd1,buffer,1);
 

 
 codigo=lseek(fd2,0,SEEK_END)+1 ;//atualiza o codigo do produto
 write(fd2,buffer,i*sizeof(char));//escreve o novo nome no strings
 lseek(fd2,0,SEEK_SET);
 
 close(fd2);
 close(fd1);

return codigo;
}

int vinteporcento(){
struct lixo lixo;
int fd3=open("lixo.txt",O_CREAT|O_RDWR,0666);
 lseek(fd3,0,SEEK_SET);
 read(fd3,&lixo,sizeof(struct lixo));
 if (lixo.total*0.2==lixo.lixo) return 1;
 close(fd3);
return 0;

}


int main(){

 if (vinteporcento()){
 int fd1=open("artigos.txt",O_CREAT|O_RDWR,0666);


 struct artigo art;
 int n; 
 int count=0;
 while((n=read(fd1,&art,sizeof(struct artigo)))>0){ //procura onde se encontra o codigo no artigos
  count+=n;
  art.codigo=crianovapos(art);
  lseek(fd1,count-sizeof(struct artigo),SEEK_SET);
  write(fd1,&art,sizeof(struct artigo));
 }}
 unlink("strings.txt");
 rename("strings2.txt","strings.txt");


return 0;	
}