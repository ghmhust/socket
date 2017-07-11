#ifndef XUDP_H
#define XUDP_H

#ifdef WIN32
#ifdef XSOCKET_EXPORTS
#define XSOCKET_API __declspec(dllexport)
#else
#define XSOCKET_API __declspec(dllimport)
#endif
#else
#define XSOCKET_API
#endif
class XSOCKET_API XUdp
{
public:
	int CreateSocket();
	void Close();
	bool Bind(unsigned short port);
	int Recv(char* buf, int bufsize);
	char ip[16];
	unsigned short port = 0;
	XUdp();
	~XUdp();

protected:
	int sock = 0;
private:
	void* addr = 0;	//用来存放client结构体
};
#endif

