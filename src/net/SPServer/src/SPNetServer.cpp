#include "SPNetServer.h"
#include <memory>
#include <utility>

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
	
	auto NewPlayer = std::make_unique <Player> (new Player()); // use reset() when end game or disconnt?
	 PlayerList.emplace_back(std::move(NewPlayer));
	// NewPlayer
	//auto col = NewPlayer->color().Value;
	//_sock.send(fmt::format("color {} {} {}\n", col.x, col.y, col.z));
	SITA_LOG("Connected");



	//isConnected = true;
	//// accept all socklist client; 
	//for (int i; i < _sockList.size(); i++) {
	//
	//	_listenSock.accept(_sockList[i]->getSocket());
	//}



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
