#include "XTcp.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#define socklen_t int
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define closesocket close
#endif

using namespace std;

int XTcp::CreateSocket(){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1){
		cout << "create socket failed!!!" << endl;
		return -1;
	}
	return sock;
}

bool XTcp::Bind(unsigned short port){
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;

	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0){
		printf("bind port %d failed!!!\n", port);
		return false;
	}
	printf("bind port %d succeed!!!\n", port);
	listen(sock, 10);		//监听10个
	return true;
}

XTcp XTcp::Accept(){
	XTcp tcp;
	sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int client = accept(sock, (sockaddr*)&caddr, &len);		//接受连接用户信息 产生新的socket
	//此socket与用户端来进行通信
	if (client <= 0) return tcp;
	char *ip = inet_ntoa(caddr.sin_addr);
	strcpy(tcp.ip, ip);
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	printf("client ip %s port %d\n", tcp.ip, tcp.port);
	printf("accept client %d\n", client);
	return tcp;
}

bool XTcp::Connect(const char* ip, unsigned short port, int timeoutms){
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	SetBlock(false);
	fd_set set;
	if(connect(sock,(sockaddr*)&saddr,sizeof(saddr)) != 0){
		FD_ZERO(&set);
		FD_SET(sock,&set);
		timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = timeoutms*1000;
		if(select(sock+1,0,&set,0,&tm) <= 0){
			printf("connect timeout or fail\n");
			printf("connect %s:%d fail!:%s\n", ip, port, strerror(errno));
			return false;
		}
	}
	SetBlock(true);
	printf("connect %s:%d success!\n", ip, port);
	return true;
}

void XTcp::Close(){
	if (sock <= 0) return;
	closesocket(sock);
	sock = 0;
}

int XTcp::Recv(char* buf, int bufsize){
	return recv(sock, buf, bufsize, 0);
}

int XTcp::Send(const char*buf, int size){
	int s = 0;
	while (s != size){
		int len = send(sock, buf + s, size - s, 0);
		s += len;
		if (len <= 0) break;
	}
	return s;
}

bool XTcp::SetBlock(bool isblock){
	if(sock <= 0) return false;
	#ifdef WIN32
		unsigned long ul = 0;		//0为阻塞
		if(!isblock) ul = 1;
		ioctlsocket(sock,FIONBIO,&ul);
	#else
		int flags = fcntl(sock,F_GETFL,0);
		if(flags<0)	return false;
		if(isblock){
			flags = flags&~O_NONBLOCK;
		}
		else{
			flags = flags|O_NONBLOCK;
		}
		if(fcntl(sock,F_SETFL,flags)!=0)	return false;
	#endif
	return true;
}

XTcp::XTcp()
{
#ifdef WIN32
	static bool first = true;
	if (first){
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
		first = false;
	}
#endif
}


XTcp::~XTcp()
{
}
