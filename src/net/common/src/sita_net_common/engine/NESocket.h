#pragma once

#include "NEPacket.h"

namespace sita {

class NetEngine;

class NESocket {
public:
	void setUserData(void* p) { _userData = p; }
	void* userData() { return _userData; }
	Socket& getSocket() { return _sock;}
	template<class T>
	void sendPacket(T& pkt) {
		
		BinSerializer se()
	}

	void close();

friend class NetEngine;
protected:
	Socket _sock;
	void* _userData = nullptr;

	size_t _totalRecvBytes = 0;
	size_t _totalSendBytes = 0;
};


} // namespace