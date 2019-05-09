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

struct top{
int codigo;
float preco;
};

struct top topvendas[20];

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
int read_pipe(int* fd,char* path,void* buf,int bufsize){
int rd=read(*fd,buf,bufsize);
  
  if (rd<=0)  {
  close(*fd);
  *fd = open(path,O_RDONLY,066);
  rd=read(*fd,buf,bufsize);
  }

return rd;
}
//--
int dividetoken(char* str,char* arr_token[]){
int j=0;
char* token;
token = strtok (str," ");
for(int i=1;i<3;i++)
arr_token[i]=NULL;
    while (token!=NULL){
    arr_token[j]=strdup(token);
    token=strtok(NULL," ");
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
//--
int seestock(char* arr_token[]){
struct stock aux1;
int n=0,value;
int fd1=open("stocks.txt",O_RDONLY);
int codigo=atoi(arr_token[0]);
if(fd1<0) perror("ERRO");  
  while((n=read(fd1,&aux1,sizeof(struct stock)))>0){ //procura onde se encontra o codigo no artigos
    if (aux1.codigo==codigo) {value=aux1.quant; break;}
      if (n==0) {
      return -1;
      }   
    
    }
if(close(fd1)<0)perror("ERRO");
return value;
}
//--
float procurapreco_main (char* arr_token[]){
struct artigo aux1;
int n;
float preco;
int fd1=open("artigos.txt",O_RDONLY);
int codigo=atoi(arr_token[0]);
  
  if(fd1<0) perror ("ERRO");
    while((n=read(fd1,&aux1,sizeof(struct artigo)))>0){ //procura onde se encontra o codigo no artigos
      if (aux1.codigo==codigo){
      preco=aux1.preco;
      } 
      if (n==0) {
      return -1;
      }
    }

if(close(fd1)<0)perror("ERRO");
return preco;
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

int preenchetop(){
  pid_t pid;
  if ((pid=fork())==0){
    
    int fdin = open ("gr.txt",O_RDONLY);
    int fdout = open("top.txt",O_WRONLY | O_CREAT, 0666);

    dup2(fdin,0);
    dup2(fdout,1);

    execlp("sort","sort","-k","2","-nr",NULL);
    close(fdin);
    close(fdout);
  }else{
 //meter wait
  wait(NULL); 
  int fdin=open("top.txt",O_RDWR|O_CREAT,0666);
  struct artigo *aux=malloc(sizeof(struct venda));
  int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
  int n=1;
  char buffer[1024];
  char *token;
  for(int i=0;i<20;topvendas[i++].codigo=-1,topvendas[i++].preco=-1);

  for (int i=0;i<20 && n>0;i++){
   //char buffer[1024];
   n=readln(fdin,buffer,1024);
   if (n==0) break;
   //puts(buffer);
   token=strtok(buffer,"\n");
   dividetoken(token,arr_token);
   topvendas[i].codigo=atoi(arr_token[0]);
   topvendas[i].preco=procurapreco(aux,fd2,topvendas[i].codigo);
   //printf("O TOP %d %f \n",topvendas[i].codigo,topvendas[i].preco );
  }
 
  close (fdin);
  close (fd2); 
 }



  return 0;
}


float procuratop(struct artigo *aux1,int fd2,int codigo){
float preco;
int i;
for(i=0;i<10;i++){
  if (topvendas[i].codigo==codigo)
    {preco=topvendas[i].preco;break;}

}
if (i==10) return -1;
return preco;


}

int addVenda (char* arrtoken[]){
 
 struct venda *aux=malloc(sizeof(struct venda));
 struct artigo *aux1=malloc(sizeof(struct artigo)); 
 float preco;
 
 int fd1=open("vendas.txt",O_CREAT|O_RDWR,0666);
 int fd2=open("artigos.txt",O_CREAT|O_RDWR,0666);
 
 if(fd1<0 || fd2<0) write(1,"ERRO\n",5);
 
 int codigo=atoi(arr_token[0]);
 int quant=abs(atoi(arr_token[1]));
 
 aux->codigo=codigo;
 aux->quant=quant;
 preco=procuratop(aux1,fd2,codigo);
 
 if(preco==-1)
 preco=procurapreco(aux1,fd2,codigo);
 
 if (preco<0) return -1;
 
 aux->ptotal=abs(preco*(float)aux->quant);
 char buff[1024];
 sprintf(buff,"%d %d %f\n",codigo,quant,aux->ptotal);
 // puts(buff);
 
 lseek(fd1,0,SEEK_END);
 write(fd1,buff,strlen(buff));
 
 if (close (fd1)<0) write(1,"ERRO\n",5);
 if (close (fd2)<0) write(1,"ERRO\n",5);
 
 return 0;
}



//--
struct request{
int stock;
float preco;

};


//--
int main(int argc,char** argv){
struct cliente aux2;
char* token;
int fifo;
int fdfifo;
preenchetop();
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
    
    if(arr_token[0]!=NULL && arr_token[1]==NULL){
    struct request r;
    char buff[1024];
    //puts("1\tcomandos");
    sprintf(buff,"%s%d","fifo",aux2.pid);
    if((r.stock=seestock(arr_token))>=0){
   // printf("O stock 1 cmd %d\n",r.stock );
    fdfifo= open(buff,O_WRONLY);
    
    
    }
/* certo*/
    if((r.preco=procurapreco_main(arr_token))>=0){   //(struct artigo *aux1,int fd2,int codigo)
    fdfifo= open(buff,O_WRONLY);
    write(fdfifo,&r,sizeof(struct request));
    close(fdfifo);
    }
    else 
    puts("Codigo not available");
  
    }
    else if (arr_token[0]!=NULL && arr_token[1]!=NULL && arr_token[2]==NULL){
    if (addVenda(arr_token)==-1) puts("Codigo not found");
    struct request r;
   // puts("2 comando");
    //if (!addVenda(arr_token)) 
    //d=updatestock(arr_token,aux2.pid);~
    char buff[PIPE_BUF];
    sprintf(buff,"%s%d","fifo",aux2.pid);
    //if (updatestock(arr_token,aux2.pid)==-1) puts("Codigo not found"); 
    //send message
/*->*/
    if ((r.stock=updatestock(arr_token,aux2.pid))>=0){
    r.preco=-1;   
    //printf("O stock é %d\n",r.stock ); 
    fdfifo=open(buff,O_WRONLY);
    write(fdfifo,&r,sizeof(struct request));
    close(fdfifo);
/*<-*/

    }
    else puts("Codigo no found");                     
    } 
    
  }


return 0;
}
