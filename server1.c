#include <netinet/in.h>    //定义数据结构sockaddr_in所需头文件
#include <sys/types.h>    //数据类型定义所需头文件
#include <sys/socket.h>   //提供socket函数及数据结构
#include <stdio.h>        
#include <stdlib.h>       
#include <string.h>       

#define SERVERPORT 9000    
# define MAXDATASIZE 1024
int main()
{
   char recBuf[MAXDATASIZE];
   char sendBuf[MAXDATASIZE];
   int sockfd, new_fd, numbytes;//定义套接字，//定义新的套接字描述符，//定义字节数
   struct sockaddr_in server_addr; //server's address information存储在套接字结构体里
   struct sockaddr_in client_addr; //client's address information存储在套接字结构体里
   int sin_size;

   server_addr.sin_family = AF_INET; //创建套接字时，用该字段指定地址家族，对于TCP/IP协议的，必须设置为AF_INET
   server_addr.sin_port = htons(SERVERPORT);  //server地址的端口号,（本机到网络字节顺序的转换：host to network short）
	//short:两个字节;long：四个字节
	//存在本机字节顺序、网络字节顺序
   server_addr.sin_addr.s_addr = INADDR_ANY;//自动填上所运行的机器的 IP 地址
   memset(&client_addr,0,sizeof(struct sockaddr));//清零，初始化
        //in代表Internet
	//h:host;to:to;n:net;s:short;l:long

   //create socket
   if ((sockfd = socket ( AF_INET , SOCK_STREAM , 0)) == - 1) { 
        printf ("socket error\n"); 
        return 1; 
    }
	//socket函数,int socket(int domain, int type, int protocol)
	//socket在错误的时候返回-1

   //bind
   if ( bind (sockfd, (struct sockaddr *) & server_addr, sizeof (struct sockaddr))!=0){
        printf ("bind failed！"); 
        return 1; 
    }
    else{
    printf("server start \n\n");
    }
	//该函数将本地主机地址以及端口号与所创建的套接字绑定起来
	//bind函数：int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
        //sockfd 是调用 socket 返回的文件描述>符。my_addr 是指向数据结//构 struct sockaddr >的指针，它保存地址(即端口和 IP 地址) 信息。
        // addrlen 设置为 sizeof(struct sockaddr)。
        //bind() 在错误的时候返回-1 
     while (1) {
     //listen
         if(listen(sockfd,6)!=0){
            printf("listen error"); 
            return 1;
     }
	//int listen(int sockfd, int backlog); 
	//sockfd 是调用 socket() 返回的套接字文件描述符。backlog 是在进入 队列中允许的连接数目。
	//listen 监听连接
	//发生错误的时候返回-1
	//此函数表示服务器愿意接受连接，20指队列中允许的最大排列请求的个数

     //accept
     sin_size = sizeof ( struct sockaddr_in );
     if((new_fd=accept(sockfd,(struct sockaddr *)&client_addr,&sin_size))==-1){ 
            printf ( "lient has disconnected！" ); 
            break ; 
        }
	//int accept(int sockfd, void *addr, int *addrlen); 
	//sockfd 是和 listen() 中一样的套接字描述符。addr 是个指向局部的数据结构sockaddr_in的指针。这是要求接入的信息所要去的地方（测定那个地址在那个端口呼叫，即客户端口号）。在它的地址传递给 accept 之 前，addrlen 是个局部的整形变量，设置为 sizeof(struct sockaddr_in)。
	//accept函数
	//在错误时返回-1
    //printf ("server: got connection from %s\n",inet_ntoa(client_addr.sin_addr));
    printf ("getting data from client...\n");
    int bAvail = 1;
    while(bAvail){
       if ((numbytes=recv(new_fd,recBuf, MAXDATASIZE, 0) ) == - 1) { 
             printf( "data receive failed or client has disconnected!\n" ); 
//       bAvail = -1;
             break; 
           }
       if(numbytes) { 
//            recBuf[numbytes] = '/0'; 
            printf ("received: %s\n" ,recBuf); 
            sleep (1);
           }
	//int recv(int sockfd, void *buf, int len, unsigned int flags); 

	//sockfd 是要读的套接字描述符。buf 是要读的信息的缓冲。len 是缓 冲的最大长度。flags 可以设置为0. recv() 返回实际读入缓冲的数据的字节数。或者在错误的时候返回-1

       //send data to client
       printf("send:");
       scanf("%s",sendBuf);
       if(send(new_fd,sendBuf,strlen(sendBuf),0)==-1){
          printf ( "send error" ); 
          return -1;
       }
       sleep (1);   
	
	//int send(int sockfd, const void *msg, int len, int flags); 
	//sockfd 是发送数据的套接字描述符(或者是调用 socket() 或者是 accept() 返回的。)msg 是指向你想发送的数据的指针。len 是数据的长度。 把 flags 设置为 0 。它在错误的时候返回-1   
    }close(new_fd); 
     } 
     close(sockfd);
     return 0; 
}
