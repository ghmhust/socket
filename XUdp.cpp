#include "XUdp.h"
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
#define closesocket close
#endif

int XUdp::CreateSocket(){
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1){
		printf("create socket failed!!!\n");
		return -1;
	}
	return sock;
}

void XUdp::Close(){
	if (sock <= 0) return;
	closesocket(sock);
	if (addr){
		delete addr;
	}
	addr = 0;
	sock = 0;
}

bool XUdp::Bind(unsigned short port){
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;

	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0){
		printf("bind port %d failed!!!\n", port);
		return false;
	}
	printf("bind port %d succeed!!!\n", port);
	listen(sock, 10);		//¼àÌý10¸ö
	return true;
}

int XUdp::Recv(char* buf, int bufsize){
	if (sock <= 0) return 0;
	if (addr == 0){
		addr = new sockaddr_in();
	}
	socklen_t len = sizeof(sockaddr_in);
	int re = recvfrom(sock, buf, bufsize, 0, (sockaddr*)addr, &len);
	return re;
}

XUdp::XUdp()
{
#ifdef WIN32
	static bool first = true;
	if (first){
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
		first = false;
	}
	memset(ip, 0, sizeof(ip));
#endif
}


XUdp::~XUdp()
{
}
