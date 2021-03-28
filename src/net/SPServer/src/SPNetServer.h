#pragma once
#include "pch.h"
#include "..\..\..\src\net\SPClient\src\Player.h" //if static lib is fine, not sure how to properly include properly from are executable file. 
class SPNetServer : NetEngine {
	public:

	void RunServer();
	virtual void onInitServer();
	virtual void onDeinitServer();
	void _update();
	void StartListen(const SockAddr& addr);
	void StopListen();
	SockAddr sockAddr;
	private:
	bool isServerRunning = true;

	private:
	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	Vector<u8>		_recvPacketBuf;
	Vector< std::unique_ptr<NESocket> > _sockList;
	Vector< std::unique_ptr<Player> > PlayerList;

	Socket _listenSock;
	Socket _sock;
	String _sockBuf;
	int port = 30000;

	char hostname[256] = "localhost";
};