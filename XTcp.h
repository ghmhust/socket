#ifndef XTCP_H
#define XTCP_H

#ifdef WIN32
#ifdef XSOCKET_EXPORTS
#define XSOCKET_API __declspec(dllexport)
#else
#define XSOCKET_API __declspec(dllimport)
#endif
#else
#define XSOCKET_API
#endif
class XSOCKET_API XTcp
{
public:
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();		//返回对象 不能再析构函数里面关闭socket
	void Close();		//关闭连接时产生的socket
	int Recv(char* buf, int bufsize);
	int Send(const char*buf, int size);
	bool Connect(const char* ip, unsigned short port, int timeoutms = 1000);
	bool SetBlock(bool isblock);
	XTcp();
	virtual ~XTcp();
	int sock = 0;
	unsigned short port = 0;
	char ip[16];
};

#endif