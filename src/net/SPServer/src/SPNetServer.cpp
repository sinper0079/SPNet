#include "SPNetServer.h"
#include <memory> 
#include <utility>
bool g_quit = true;
void SPNetServer::RunServer()
{
	onInitServer();
	while (isServerRunning) {
		update();
	}

	onDeinitServer();
}

void SPNetServer::UpdateListenPoll() {
	if (!g_quit) {
		auto n = PlayerConnects.size();
		_pollfds.resize(n + 1);

		{
			for (size_t i = 0; i < PlayerConnects.size(); i++) {
				auto& c = PlayerConnects[i];
				if (!c->isValid()) {
					if (PlayerConnects.size() > 1)
						std::swap(c, PlayerConnects.back());
					PlayerConnects.resize(PlayerConnects.size() - 1);
					i--;
					continue;
				}
				c->getPollFD(_pollfds[i]);
			}
		}

		_pollfds[n].reset(_listenSock, true, false);

		int ret = my_poll(_pollfds.data(), _pollfds.size(), 1000);

		{
			int i = 0;
			for (auto& c : PlayerConnects) {
				try {
					c->onUpdate(_pollfds[i]);
				}
				catch (...) {
					c->close();
				}
				i++;
			}
		}

		if (_pollfds[n].canRead()) {
			PlayerConnects.emplace_back(new ClientConnect);
			auto& newClient = PlayerConnects.back();
			newClient->_server = this;
			newClient->acceptFromListenSock(_listenSock);
			SITA_LOG("Accepeted new Client");
		}
	}
}
void SPNetServer::StartListen(const SockAddr& addr)
{
	SITA_LOG("Start Listening");
	_listenSock.createTCP();
	_listenSock.setNonBlocking(true);
	_listenSock.bind(addr);
	_listenSock.listen();	
	_listenSock.accept(_sock);
	auto NewPlayer = std::make_unique <Player>();	// use reset() when end game or disconnt?
	 PlayerList.emplace_back(std::move(NewPlayer)); //move into vector, no copy , original ptr will be empty 

	 //if (PlayerList[PlayerList.size()- 1]) {
		// auto col = PlayerList[PlayerList.size() - 1]->color().Value;
		// _sock.send(fmt::format("color {} {} {}\n", col.x, col.y, col.z));
	 //}

	 g_quit = false;






}
void SPNetServer::StopListen()
{


}
void SPNetServer::onRecv(std::unique_ptr<NESocket>& s)
{

	if (!s) return;

	NEPacketHeader hdr;

	const size_t _maxPacketPerFrame = 100;

	_recvPacketBuf.clear();
	_recvPacketBuf.resize(_maxPacketSize);

	for (size_t i = 0; i < _maxPacketPerFrame; i++) {
		auto bytesToRead = s->getSocket().availableBytesToRead();
		if (bytesToRead < sizeof(hdr.len)) {
			break;
		}

		int ret = s->getSocket().recv(_recvPacketBuf, sizeof(hdr.len), MSG_PEEK);//buffer not clear == size hdr
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

		ret = s->getSocket().recv(_recvPacketBuf, hdr.len, 0); //socket buf read
		if (ret <= 0) {
			// error or disconnected
		}

		{
			BinDeserializer se(_recvPacketBuf);
			se.io(hdr);
			//onRecvPacket(s, hdr, _recvPacketBuf.data()); //which socket 
		}
	}


}
void SPNetServer::onDeinitServer()
{
	SITA_LOG("onDeinitServer");
	StopListen();
	_listenSock.close();
	_sock.close();
	isServerRunning = false;
}

void SPNetServer::update()
{

	__super::update();
	UpdateListenPoll();
	for (int i; i< _sockList.size();i++) {
		// how to know sock have something to read
		onRecv(_sockList[i]); // non copyable only can pass ref?
	}

}

void SPNetServer::onInitServer()
{
	SITA_LOG("onInitServer");
	sockAddr.resolve(hostname, static_cast<uint16_t>(port));
	StartListen(sockAddr);
}
