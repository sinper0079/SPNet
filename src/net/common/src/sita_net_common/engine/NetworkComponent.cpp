#include "NetworkComponent.h"

void sita:: NetworkComponent::connect(const char* hostname, uint16_t port)
{
	_sock.createTCP();
	_sock.setNonBlocking(true);
	if (_sock.connect(hostname, port)) {
		_onConnected();
		return;
	}
	_status = Status::Connecting;
	SITA_LOG("connecting\n");
}

void  sita::NetworkComponent::_onConnected()
{
	_status = Status::Connected;
	SITA_LOG("_onConnected\n");
	onConnected();
}

void  sita::NetworkComponent::acceptFromListenSock(Socket& listenSock)
{
	listenSock.accept(_sock);
	_pollfd.fd = _sock.sock();
	_onConnected();
}

void  sita::NetworkComponent::sendPacket(SPPacket& pkt)
{
	auto type = pkt.type();
	pkt.toBuffer(_tmp);

	//printf("send packet %d\n", enum_int_ref(type));


		SITA_LOG("send packet {}", enum_int_ref(type));
	//append to sendBuf
	_sendBuf.insert(_sendBuf.end(), _tmp.begin(), _tmp.end());
}

void  sita::NetworkComponent::_onRecv()
{
	auto bytesInSocket = _sock.availableBytesToRead();
	if (!bytesInSocket) {
		close();
		return;
	}

	const auto headerSize = sizeof(SPPacket::PacketSize) + sizeof(SPPacketType);

	// recv header
	if (_recvBuf.size() < headerSize) {
		auto bytesToRead = headerSize - _recvBuf.size();
		if (bytesInSocket < bytesToRead) {
			bytesToRead = bytesInSocket;
		}


		_sock.appendRecv(_recvBuf, bytesToRead,0); // not sure 
		//_sock.appendRecv(_recvBuf, bytesToRead);
		bytesInSocket -= bytesToRead;
	}

	// not enough for header
	if (_recvBuf.size() < headerSize)
		return;

	// recv body
	BinDeserializer de(_recvBuf);
	SPPacket::PacketSize packetSize;
	SPPacketType packetType;

	de.io_fixed(packetSize);
	de.io_fixed(enum_int_ref(packetType));

	if (packetSize < headerSize)
		throw SITA_ERROR("invalid header");

	auto bytesToRead = packetSize - headerSize;
	if (bytesInSocket < bytesToRead) {
		bytesToRead = bytesInSocket;
	}

	_sock.appendRecv(_recvBuf, bytesToRead,0);

	// not enough for body
	if (_recvBuf.size() < packetSize)
		return;

	assert(_recvBuf.size() == packetSize);
	SITA_LOG("onRecvPacket {}", enum_int_ref(packetType));
	onRecvPacket(packetType, _recvBuf);
	_recvBuf.clear();
}

void  sita::NetworkComponent::_onSend()
{
	if (_status == Status::Connecting) {
		_onConnected();
	}

	if (_sendBufOffset >= _sendBuf.size()) {
		_sendBufOffset = 0;
		_sendBuf.clear();
		return;
	}

	auto n = _sendBuf.size() - _sendBufOffset;
	auto ret = _sock.send(&_sendBuf[_sendBufOffset], n);
	if (ret <= 0) {
		close();
		return;
	}

	_sendBufOffset += ret;
}

void  sita::NetworkComponent::onUpdate(MyPollFD& fd)
{
	if (fd.canRead())
		_onRecv();
	if (fd.canWrite())
		_onSend();
}

void  sita::NetworkComponent::getPollFD(MyPollFD& pf)
{
	bool write = false;
	if (_status == Status::Connecting)
		write = true;
	if (_status == Status::Connected && _sendBuf.size())
		write = true;

	pf.reset(_sock, true, write);
}

void  sita::NetworkComponent::close()
{
	_status = Status::Closed;
	_sock.close();
	SITA_LOG("closed\n");
}

bool sita::NetworkComponent::isValid()
{
	return _sock.isValid();
}
