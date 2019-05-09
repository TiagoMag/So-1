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
#include <sys/wait.h>


int array[20];

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

ssize_t readln (int fd, void *buf, size_t nbyte) {
    nbyte--;
    char *cbuf = (char*) buf;
    int i;
    int rd=1;
    off_t foundnl = 0;

    rd = read (fd, cbuf, nbyte);
    for (i = 0; i < rd; i++)
        if (cbuf[i] == '\n'){
            foundnl = 1;
            break;
        }
    cbuf[i] = 0;
    lseek (fd, (i - rd) + foundnl, SEEK_CUR);

    return (foundnl == 1 ? i : -i);
}

struct venda{
 int codigo;
 int quant;
 float ptotal;
};

struct vendaR{
 int codigo;
 int quant;
 int pid;
 float ptotal;
};

int contalinhas (){
 int i=0;
  
 int fd1=open("vendas.txt",O_RDWR,0666);

char buffer[4024];
//printf("XX%ld\n",readln(fd1,buffer,4024) );
 while ((readln(fd1,buffer,4024))>0){
 	i++;
 }
 close(fd1);

return i;
}

int validacod (int cod,int array[],int N){
for(int i=0;i<N;i++){
 if (cod==array[i] && array[i]!=-1) return 1;	
}

return 0;

}

int divideagrega(){
 int d;
 d=contalinhas();
 
 int lines=d;
 int linhas=0;

 for (int i = 1; i < 2; i++,linhas+=lines) {
  if (!fork()) {           
   
   struct venda auxt;
   struct venda aux;
   struct venda aux2;
   char buffer[1024];
   int l=0;
   
   for (int i=0;i<20;array[i++]=-1);
   
   int count=1;
   int p;
   int lines2=lines+1;
  
   int fd1=open("vendas.txt",O_RDWR,0666);
   int h=0;
   while(count<=lines){
    if(count>=linhas && count<=linhas+lines ){
    
    lines2--;
 
    h+=readln(fd1,buffer,1024);
   
    char* arr_token[3];
    int g;
    
    char* token;
    token=strtok(buffer,"\n");
    dividetoken(token,arr_token);
    aux.quant=atoi(arr_token[1]);
    aux.ptotal=atoi(arr_token[2]);
    aux.codigo=atoi(arr_token[0]);
    auxt.quant=aux.quant;
    auxt.ptotal=aux.ptotal;
    auxt.codigo=aux.codigo;
    
    g=lseek(fd1,0,SEEK_CUR);
 
     for(int j=1;j<lines2 && !validacod(auxt.codigo,array,20);j++){
      p+=readln(fd1,buffer,1024);
   
    
      token=strtok(buffer,"\n");
      dividetoken(token,arr_token);
     	
      aux2.quant=atoi(arr_token[1]);
      aux2.ptotal=atoi(arr_token[2]);
      aux2.codigo=atoi(arr_token[0]);
       if(aux.codigo==aux2.codigo){
      
         auxt.quant+=aux2.quant;
         auxt.ptotal+=aux2.ptotal;
       }
     }
     lseek(fd1,g,SEEK_SET);
     
     if (!validacod(auxt.codigo,array,20)){
    

      char buff5[1024];
      sprintf(buff5,"%s","gr.txt"); //passa um int para char
      int fd12=open(buff5,O_CREAT|O_RDWR,0666);
      char buffer4[256];  sprintf(buffer4,"%d %d %f\n",auxt.codigo,auxt.quant,auxt.ptotal);
      lseek(fd12,0,SEEK_END);
      write(fd12,buffer4,strlen(buffer4));
      close(fd12);
    
      array[l++]=auxt.codigo;
    

     }
     
    count++;
    
   } 
  }
 }
}

 return 0;
}





int main(int argc,char** argv){

divideagrega();

 return 0;
}