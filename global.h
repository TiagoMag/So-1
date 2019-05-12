#ifndef GLOBAL_H
#define GLOBAL_H

struct artigo{
 int codigo;
 float preco;
};

struct mudaPreco{
 int flag;
 float preco;
 int codigo;
};

struct message_client{
int pid;
char buffer [50];
};

struct request_client{
int stock;
float preco;
};

struct stock{
 int codigo;
 int quant;
};


struct venda{
 int codigo;
 int quant;
 float ptotal;
};


struct top{
int codigo;
float preco;
};

struct lixo{
int lixo;
int total;

};
//API
ssize_t readln2 (int fd, void *buf, size_t nbyte);

ssize_t readln (int fd, void *buf, size_t nbyte);

int dividetoken(char* str,char* arr_token[]);

int dividetoken2(char* str,char* arr_token[]);

int read_pipe(int* fd,char* path,void* buf,int bufsize);

#endif