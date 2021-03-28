#include "NetEngine.h"

namespace sita {



	void NetEngine::listen(const NESocketAddr& addr)
	{

		SITA_LOG("Start Listening");
		_listenSock.createTCP();
		_listenSock.setNonBlocking(true);
		//_listenSock.setReuseAddr(true);
		//addr.
		//addr._addr.sa_data setIPv4(0, 0, 0, 0);
		//addr.setPort(3300);
		_listenSock.bind(hostname, static_cast<uint16_t>(port));
		_listenSock.listen();
		_listenSock.accept(_sock);

	}
	void NetEngine::stopListen()
	{
	}
	NESocket* NetEngine::connect(const NESocketAddr& addr)
	{
		return nullptr;
	}
	
	void NetEngine::onRecvPacket(NESocket* s, const NEPacketHeader& hdr, const u8* data)
	{
	}

//	void NetEngine::onRecv(NESocket* s) {
//	if (!s) return;
//
//	NEPacketHeader hdr;
//
//	const size_t _maxPacketPerFrame = 100;
//
//	_recvPacketBuf.clear();
//	_recvPacketBuf.resize(_maxPacketSize);
//
//	for (size_t i = 0; i < _maxPacketPerFrame; i++) {
//		auto bytesToRead = s->_sock.availableBytesToRead();
//		if (bytesToRead < sizeof(hdr.len)) {
//			break;
//		}
//
//		int ret = s->_sock.recv(_recvPacketBuf, sizeof(hdr.len), MSG_PEEK);
//		if (ret <= 0) {
//			// error or disconnected
//		}
//
//		{
//			BinDeserializer se(_recvPacketBuf);
//			se.io_fixed(hdr.len);
//		}
//		
//		if (bytesToRead < hdr.len)
//			break;
//
//		ret = s->_sock.recv(_recvPacketBuf, hdr.len, 0);
//		if (ret <= 0) {
//			// error or disconnected
//		}
//
//		{
//			BinDeserializer se(_recvPacketBuf);
//			se.io(hdr);
//			onRecvPacket(s, hdr, _recvPacketBuf.data());
//		}
//	}
//
//}




} // namespace