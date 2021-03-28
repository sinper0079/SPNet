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
void SPNetServer::onDeinitServer()
{
	SITA_LOG("onDeinitServer");
	stopListen();
	_listenSock.close();
	_sock.close();
	isServerRunning = false;
}

void SPNetServer::onInitServer()
{
	SITA_LOG("onInitServer");
	listen(sockAddr);
}
