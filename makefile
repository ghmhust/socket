libxsocket.so: XTcp.h XTcp.cpp
	g++ $+ -o $@ -fpic -shared -std=c++11