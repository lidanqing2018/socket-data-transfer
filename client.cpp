// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>   

#pragma comment(lib,"ws2_32.lib")

#define DES_IP "192.168.109.131"    //目的服务器IP
#define DES_PORT 9000               //端口号

int main()
{
	SOCKET sock;         //socket套接字           
	char buf[1024];      //缓冲区
	SOCKADDR_IN ser_addr; //server's address information存储在套接字结构体里
	WSADATA wsaData;      //存放windows socket初始化信息的数据结构
	WORD wVersionRequired;   //初始化socket
	wVersionRequired = MAKEWORD(2, 2);  //初始化socket
	if (WSAStartup(wVersionRequired, &wsaData) != 0)  //初始化socket
	{
		printf("init failed %ld\n", WSAGetLastError());

		//  return 1;
	}

	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);              //socket函数,int socket(int domain, int type, int protocol)
														//socket在错误的时候返回-1
	ser_addr.sin_addr.S_un.S_addr = inet_addr(DES_IP);  //192.168.109.131
	ser_addr.sin_port = htons(DES_PORT);                //9000
	ser_addr.sin_family = AF_INET;		               //创建套接字时，用该字段指定地址家族，对于TCP/IP协议的，必须设置为AF_INET
	memset(ser_addr.sin_zero, 0, 8);

	//connect server
	if (connect(sock, (SOCKADDR *)&ser_addr, sizeof(SOCKADDR_IN)) != 0)
	{
		int nError = WSAGetLastError();
		printf("connect server failed! error:%d\n\n", WSAGetLastError());
		system("pause");
		return 1;
	}
	//int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); 	
	//sock 是系统调用 socket() 返回的套接字文件描述符。ser_addr 是 保存着目的地端口和 IP 地址的数据结构 struct sockaddr。addrlen 设置 为 sizeof(SOCKADDR_IN)。 
	//connect 用于建立连接，sever_addr是保存着服务器IP地址和端口号的数据结构struct sockaddr.
	//connect() 的返回值--它在错误的时候返回-1
	else
	{
		printf("connected server，send data now.press exit to end.\n\n");
	}

	//send data
	while (!NULL)
	{
		int SentBytes;
		int RecvBytes;
		char strRecv[1024] = { 0 };
		printf("send:");
		scanf("%s", buf);
		if (strcmp(buf, "exit") == 0) break;
		
		
		SentBytes = send(sock, buf, strlen(buf), 0);
		if (SentBytes == SOCKET_ERROR)
		{
			printf("send failed！\n");
			break;
		}
		//int send(int sockfd, const void *msg, int len, int flags); 
		//sockfd 是发送数据的套接字描述符(或者是调用 socket() 或者是 accept() 返回的。)msg 是指向你想发送的数据的指针。len 是数据的长度。 把 flags 设置为 0 。它在错误的时候返回-1

		//receive the data from server
		if ((RecvBytes = recv(sock, strRecv, sizeof(strRecv), 0)) != SOCKET_ERROR)
		{
			strRecv[RecvBytes] = 0;
			printf("recive: %s\n", strRecv);
		}
		else
		{
			printf("receive failed！%d\n", WSAGetLastError());

		}
		//int recv(int sockfd, void *buf, int len, unsigned int flags); 

		//sockfd 是要读的套接字描述符。buf 是要读的信息的缓冲。len 是缓 冲的最大长度。flags 可以设置为0. recv() 返回实际读入缓冲的数据的字节数。或者在错误的时候返回-1
		Sleep(1000);
	}
	closesocket(sock); //关闭连接
	WSACleanup();
	return 0;
}




