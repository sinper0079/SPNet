#include "SPNetServer.h"

void SPNetServer::RunServer()
{
	onInitServer();
	while (isServerRunning) {
		_update();
	}

	onDeinitServer();
}
void SPNetServer::_update()
{


}
void SPNetServer::StartListen(const SockAddr& addr)
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
void SPNetServer::StopListen()
{


}
void SPNetServer::onDeinitServer()
{
	SITA_LOG("onDeinitServer");
	StopListen();
	_listenSock.close();
	_sock.close();
	isServerRunning = false;
}

void SPNetServer::onInitServer()
{
	SITA_LOG("onInitServer");
	StartListen(sockAddr);
}
