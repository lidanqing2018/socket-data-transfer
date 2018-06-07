// client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>   

#pragma comment(lib,"ws2_32.lib")

#define DES_IP "192.168.109.131"    //Ŀ�ķ�����IP
#define DES_PORT 9000               //�˿ں�

int main()
{
	SOCKET sock;         //socket�׽���           
	char buf[1024];      //������
	SOCKADDR_IN ser_addr; //server's address information�洢���׽��ֽṹ����
	WSADATA wsaData;      //���windows socket��ʼ����Ϣ�����ݽṹ
	WORD wVersionRequired;   //��ʼ��socket
	wVersionRequired = MAKEWORD(2, 2);  //��ʼ��socket
	if (WSAStartup(wVersionRequired, &wsaData) != 0)  //��ʼ��socket
	{
		printf("init failed %ld\n", WSAGetLastError());

		//  return 1;
	}

	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);              //socket����,int socket(int domain, int type, int protocol)
														//socket�ڴ����ʱ�򷵻�-1
	ser_addr.sin_addr.S_un.S_addr = inet_addr(DES_IP);  //192.168.109.131
	ser_addr.sin_port = htons(DES_PORT);                //9000
	ser_addr.sin_family = AF_INET;		               //�����׽���ʱ���ø��ֶ�ָ����ַ���壬����TCP/IPЭ��ģ���������ΪAF_INET
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
	//sock ��ϵͳ���� socket() ���ص��׽����ļ���������ser_addr �� ������Ŀ�ĵض˿ں� IP ��ַ�����ݽṹ struct sockaddr��addrlen ���� Ϊ sizeof(SOCKADDR_IN)�� 
	//connect ���ڽ������ӣ�sever_addr�Ǳ����ŷ�����IP��ַ�Ͷ˿ںŵ����ݽṹstruct sockaddr.
	//connect() �ķ���ֵ--���ڴ����ʱ�򷵻�-1
	else
	{
		printf("connected server��send data now.press exit to end.\n\n");
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
			printf("send failed��\n");
			break;
		}
		//int send(int sockfd, const void *msg, int len, int flags); 
		//sockfd �Ƿ������ݵ��׽���������(�����ǵ��� socket() ������ accept() ���صġ�)msg ��ָ�����뷢�͵����ݵ�ָ�롣len �����ݵĳ��ȡ� �� flags ����Ϊ 0 �����ڴ����ʱ�򷵻�-1

		//receive the data from server
		if ((RecvBytes = recv(sock, strRecv, sizeof(strRecv), 0)) != SOCKET_ERROR)
		{
			strRecv[RecvBytes] = 0;
			printf("recive: %s\n", strRecv);
		}
		else
		{
			printf("receive failed��%d\n", WSAGetLastError());

		}
		//int recv(int sockfd, void *buf, int len, unsigned int flags); 

		//sockfd ��Ҫ�����׽�����������buf ��Ҫ������Ϣ�Ļ��塣len �ǻ� �����󳤶ȡ�flags ��������Ϊ0. recv() ����ʵ�ʶ��뻺������ݵ��ֽ����������ڴ����ʱ�򷵻�-1
		Sleep(1000);
	}
	closesocket(sock); //�ر�����
	WSACleanup();
	return 0;
}




