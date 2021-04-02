#include "NetEngine.h"

namespace sita {
void NetEngine::update() {
	
}
void NetEngine::sendPacket(std::unique_ptr<NESocket>& s, SPPacket& pkt)
{
	_sendPacketBuf.clear();
	BinSerializer se(_sendPacketBuf);
	se.io(pkt);
	send(s, _sendPacketBuf.data(), _sendPacketBuf.size());
}

void NetEngine::onRecv(NESocket* s) {
	if (!s) return;

	NEPacketHeader hdr;

	const size_t _maxPacketPerFrame = 100;

	_recvPacketBuf.clear();
	_recvPacketBuf.resize(_maxPacketSize);

	for (size_t i = 0; i < _maxPacketPerFrame; i++) {
		auto bytesToRead = s->_sock.availableBytesToRead();
		if (bytesToRead < sizeof(hdr.len)) {
			break;
		}

		int ret = s->_sock.recv(_recvPacketBuf, sizeof(hdr.len), MSG_PEEK);//buffer not clear == size hdr
		if (ret <= 0) {
			// error or disconnected
		}
		if (ret < sizeof(hdr.len)) {
			// error
		}
		{
			BinDeserializer se(_recvPacketBuf);
			se.io_fixed(hdr.len);
		}
		
		if (bytesToRead < hdr.len) //socket rec complete?
			break;

		ret = s->_sock.recv(_recvPacketBuf, hdr.len, 0); //socket buf read
		if (ret <= 0) {
			// error or disconnected
		}

		{
			BinDeserializer se(_recvPacketBuf);
			se.io(hdr);
			onRecvPacket(s, hdr, _recvPacketBuf.data()); //which socket 
		}
	}

}

void NetEngine::acceptFromListenSock(Socket& listenSock)
{
	listenSock.accept(_sock);
	_pollfd.fd = _sock.sock();
	_onConnected();
}

void NetEngine::onUpdate(MyPollFD& fd)
{
	if (fd.canRead())
	{
	}
	//onRecv()
	//if (fd.canWrite())
	//	_onSend();
}

void NetEngine::getPollFD(MyPollFD& pf)
{
	bool write = false;
	if (_status == Status::Connecting)
		write = true;
	/*if (_status == Status::Connected && _sendBuf.size())
		write = true;*/

	pf.reset(_sock, true, write);
}



void NetEngine::send(std::unique_ptr<NESocket>& s, const u8* data, size_t dataSize)
{

	s->_sock.send(data, dataSize);

	auto ret = s->_sock.send(data, dataSize);
	if (ret <= 0) {
		s->close(); //need to add some logic to reconnect?
		return;
	}
}

void NetEngine::close()
{
	_sock.close();
	SITA_LOG("closed\n");
}

bool NetEngine::isValid()
{
	return _sock.isValid();
}

void NetEngine::_onConnected()
{
	_status = Status::Connected;
	SITA_LOG("_onConnected\n");
	onConnected();
}



} // namespace