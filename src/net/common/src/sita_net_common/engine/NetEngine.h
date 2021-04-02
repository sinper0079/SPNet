#pragma once

#include "NESocket.h"
#include "SPPacket.h"
namespace sita {

class NetEngine : public NonCopyable {
public:

	void listen(const SockAddr& addr);
	void stopListen();

	virtual void update();

	NESocket* connect(const SockAddr& addr);
	virtual void onConnected() {}
	virtual void onAccept		(NESocket* s) {}
	virtual void onConnect		(NESocket* s) {}
	virtual void onDisconnect	(NESocket* s) {}

	virtual void onRecvPacket(NESocket* s, const NEPacketHeader& hdr, const u8* data) {};
	virtual void sendPacket(std::unique_ptr<NESocket>& s, SPPacket& pkt);
	virtual void onRecv			(NESocket* s);

	void acceptFromListenSock(Socket& listenSock);

	enum class Status {
		None,
		Connecting,
		Connected,
		Closed,
	};

	void send(std::unique_ptr<NESocket>& s, const u8* data, size_t dataSize);
	void close();
	bool isValid();

private:
	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;
	Socket _sock;
	MyPollFD _pollfd;


	Vector<u8>		_sendPacketBuf;
	Vector<u8>		_recvPacketBuf;
	Vector< std::unique_ptr<NESocket> > _sockList;

	void _onConnected();

	Status _status = Status::None;
};

} // namespace