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

struct venda{
 int codigo;
 int quant;
 float ptotal;
};

int array[100000];
struct venda* arrayagrega[1000000];

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



struct vendaR{
 int codigo;
 int quant;
 int pid;
 float ptotal;
};

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

int divideagrega(){
 int d;
 d=contalinhas();
 printf("Linhas:%d\n",d );
 int lines=d;
 int linhas=0;
 
    for (int i=0;i<100000;i++){
      arrayagrega[i]=malloc(sizeof(struct venda));
      arrayagrega[i]->codigo=-1;
           arrayagrega[i]->quant=0;
                arrayagrega[i]->ptotal=0;
    }

 for (int i = 1; i < 2; i++,linhas+=lines) {
 // if (!fork()) {           
   int flag=0;
   struct venda auxt;
   struct venda aux;
   struct venda aux2;
   char buffer[1024];
   int l=0;
  
   for (int i=0;i<100000;array[i++]=-1);

   int count=1;
   int p;
   int lines2=lines+1;
  
   //int fd1=open("vendas.txt",O_RDWR,0666);
   int h=0;
   

 
   while(count<=lines && (h+=readln(0,buffer,1024))>0){
    if(count>=linhas && count<=linhas+lines ){
  
    lines2--;

    //h+=readln(0,buffer,1024);
   
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
    
    g=lseek(0,0,SEEK_CUR);
     
     if (!validacod(auxt.codigo,array,100000)){
     for(int j=1;j<lines2;j++){
      p+=readln(0,buffer,1024);

    
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
     lseek(0,g,SEEK_SET);
    
    
      //int fd12;
      //char output[1024];
      //char buff5[1024];
      //sprintf(buff5,"%s","gr.txt"); //passa um int para char
      //sprintf(output,"%d-%d-%d%s%d:%d:%d",timeinfo->tm_year + 1900,timeinfo->tm_mon + 1,timeinfo->tm_mday,"T", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      //if (flag==0) fd12=open(output ,O_CREAT|O_RDWR,0666);
      //else fd12=open(buff5,O_CREAT|O_RDWR,0666);
      
     // char buffer4[256];  sprintf(buffer4,"%d %d %f\n",auxt.codigo,auxt.quant,auxt.ptotal);
     // lseek(fd12,0,SEEK_END);
     // write(fd12,buffer4,strlen(buffer4));
     //close(fd12);

      arrayagrega[i++]->codigo=auxt.codigo;
      arrayagrega[i++]->quant=auxt.quant;
      arrayagrega[i++]->ptotal=auxt.ptotal;
      array[auxt.codigo]=1;
   
     }
   
    count++;
    //printf("%d\n",count );
 
    
   } 
  }
 //}
}

 return 0;
}





int main(int argc,char** argv){

divideagrega();
   int fd12;
   time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
      char output[1024];
      char buff5[1024];
      sprintf(buff5,"%s","gr.txt"); //passa um int para char
      sprintf(output,"%d-%d-%d%s%d:%d:%d",timeinfo->tm_year + 1900,timeinfo->tm_mon + 1,timeinfo->tm_mday,"T", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    //  if (flag==0) 
      fd12=open(output ,O_CREAT|O_RDWR,0666);
      //else 
      //fd12=open(buff5,O_CREAT|O_RDWR,0666);
      for(int i=0;i<100000 && arrayagrega[i]->codigo!=-1;i++){
      char buffer4[256];  
      sprintf(buffer4,"%d %d %f\n",arrayagrega[i]->codigo,arrayagrega[i]->quant,arrayagrega[i]->ptotal);
      lseek(fd12,0,SEEK_END);
      write(fd12,buffer4,strlen(buffer4));
     close(fd12);}
 return 0;
}