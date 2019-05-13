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

 char* arr_token[3];
 int j=0;

 for(int z=0;z<offset;z++)
 readln(0,buffer,50);

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
 char buffer[50];

 while ((readln(0,buffer,50))>0){
  i++;
 }

lseek(0,0,SEEK_SET);


return i;

}

int validacod (int cod,int array[],int N){
if (array[cod]==1) return 1;

return 0;

}

int agrega(int d){


 int lines;
 if (d>100000) lines=100000;
 else lines=d;

 int linhas=0;
 
 int fd12;
 char output[1024];
 char buff5[1024];
 
 int offset=0;
 int forks;
 if (d<=100000) forks=1;
 else forks=d/100000;


for (int m = 1; m < forks+1; m++,offset+=100000) {

  if (!fork()) { 
 
   struct venda* vendas[100000];
   int array[100000];  
    
   carregaMemoria(offset,vendas,array);


   struct venda auxt;
   struct venda aux;
   struct venda aux2;
   char buffer[1024];


   int i=0;
   int p;
   int lines2=lines+1;
 
   int z=0;
        int h=0; 
 
  while(i<=lines && vendas[i]->codigo!=-1){
  
    if(i>=linhas && i<=linhas+lines ){

     lines2--;
     //printf("%d\n",lines2 );
     auxt.codigo=vendas[i]->codigo;
     auxt.quant=vendas[i]->quant;
     auxt.ptotal=vendas[i]->ptotal;
     
     if (validacod(vendas[i]->codigo,array,100000)==0) {
    
     for(int j=0,z=i+1;j<lines2 && z<lines;j++,z++){
     
       if(vendas[z]->codigo==vendas[i]->codigo){
        
         auxt.codigo=vendas[z]->codigo;
       
         auxt.quant+=vendas[z]->quant;
         
         auxt.ptotal+=vendas[z]->ptotal;

        }
         
     }
     
      
      char buffer4[256];  
      sprintf(buffer4,"%d %d %f\n",auxt.codigo,auxt.quant,auxt.ptotal);
     
      write(1,buffer4,strlen(buffer4));
   
      
      array[auxt.codigo]=1;
   
     }
   
 
 
   } 
    i++;

  }

 _exit(m);
 }
 
}

int pid, status;
    while ((pid = wait(&status)) != -1) {
      
}

 return 0;
}








int main () {
 int d=contalinhas();

 agrega(d);

 return 0;	
}