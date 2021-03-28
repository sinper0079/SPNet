#pragma once
#include "pch.h"

class SPNetServer : NetEngine {
	public:

	void RunServer();
	virtual void onInitServer();
	virtual void onDeinitServer();
	void _update();
	void StartListen(const SockAddr& addr);
	void StopListen();
	private:
	bool isServerRunning = true;

	private:

	Socket _listenSock;
	Socket _sock;
	String _sockBuf;
	int port = 30000;
	SockAddr sockAddr;
	char hostname[256] = "localhost";
};