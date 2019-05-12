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
#include <time.h>
#include "global.h"





int carregaMemoria(int offset,struct venda* vendas[100000],int array[100000] ){
int i=0;
char buffer[1024];
printf("Ofsset %d\n",offset );
char* arr_token[3];
int j=0;

for(int z=0;z<offset;z++){
readln(0,buffer,50);

//printf("Buffer%s\n",buffer );
}

 while ( i<100000 && (readln(0,buffer,50))>0 ){
  
   
  vendas[i]=malloc(sizeof(struct venda));
  char* token;
  token=strtok(buffer,"\n");

  dividetoken(token,arr_token);
  vendas[i]->codigo=atoi(arr_token[0]);
  vendas[i]->quant=atoi(arr_token[1]);
  vendas[i]->ptotal= atof(arr_token[2]);
 i++;
 }
 printf("I %d\n",i );
 for (int j=i;i<100000;i++){
 vendas[i]=malloc(sizeof(struct venda));
   vendas[i]->codigo=-1;
  vendas[i]->quant=0;
  vendas[i]->ptotal= 0; 	
 }

return 0;
}



int contalinhas (){
 int i=0;
 //int file_desc = open("agg.txt",O_WRONLY | O_CREAT,0666); 
 // dup2(2,)
// int fd1=open("vendas.txt",O_RDWR,0666);

char buffer[50];
//printf("XX%ld\n",readln(fd1,buffer,4024) );
 while ((readln(0,buffer,50))>0){
  i++;
 }
lseek(0,0,SEEK_SET);
// close(0);

return i;
}

int validacod (int cod,int array[],int N){
if (array[cod]==1) return 1;

return 0;

}

int agrega(int d){

//int d;
 //d=contalinhas();
 int lines;
 if (d>100000) lines=100000;
 else lines=d;

 printf("lines %d\n",lines );
 
 int linhas=0;
 
 time_t rawtime;
 struct tm * timeinfo;

 time ( &rawtime );
 timeinfo = localtime ( &rawtime );
    

  int fd12;
  char output[1024];
  char buff5[1024];
  sprintf(buff5,"%s","gr.txt"); //passa um int para char
  sprintf(output,"%d-%d-%d%s%d:%d:%d",timeinfo->tm_year + 1900,timeinfo->tm_mon + 1,timeinfo->tm_mday,"T", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
 
 int offset=0;
 int forks;
 forks=d/100000;
 printf("Nmr forks %d\n",forks );

for (int m = 1; m < forks+1; m++,offset+=100000) {

  if (!fork()) { 
 
   struct venda* vendas[100000];
   int array[100000];  
    
   //carregaMemoria(offset,vendas,array);
   
   int flag=0;
   struct venda auxt;
   struct venda aux;
   struct venda aux2;
   char buffer[1024];

   int i=0;
   for (i=0;i<100000;array[i++]=-1);

   i=0;
   int p;
   int lines2=lines+1;

  int z=0;
         
 
  while(i<=lines && i<100000 && vendas[i]->codigo!=-1){
  
    if(i>=linhas && i<=linhas+lines ){

     lines2--;
     auxt.codigo=vendas[i]->codigo;
     auxt.quant=vendas[i]->quant;
     auxt.ptotal=vendas[i]->ptotal;
 
     if (!validacod(vendas[i]->codigo,array,100000)) {
    
     for(int j=0,z=i+1;j<lines2 && z<100000;j++,z++){
     
   

     
       if(vendas[z]->codigo==vendas[i]->codigo){
        
         auxt.codigo=vendas[z]->codigo;
         auxt.quant+=vendas[z]->quant;
         auxt.ptotal+=vendas[z]->ptotal;
    

       
       }
         
     }
     
     
      if (flag==0) fd12=open(output ,O_CREAT|O_RDWR|O_APPEND,0666);
      else fd12=open(buff5,O_CREAT|O_WRONLY|O_APPEND,0666);
      
      char buffer4[256];  
      sprintf(buffer4,"%d %d %f\n",auxt.codigo,auxt.quant,auxt.ptotal);
     
      write(fd12,buffer4,strlen(buffer4));
      close(fd12);
      
      array[auxt.codigo]=1;
   
     }
   
 
 
   } 
    i++;

  }
  printf("A %d\n",i );
 _exit(m);
 }
 
}

int pid, status;
    while ((pid = wait(&status)) != -1) {
        printf("O filho %d morreu com codigo de saida %d\n", pid, WEXITSTATUS(status));
}

 return 0;
}








int main () {
int d=contalinhas();
printf("Linhas %d\n",d );
agrega(d);

 return 0;	
}