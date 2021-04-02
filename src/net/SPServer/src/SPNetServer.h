#pragma once
#include "pch.h"
#include "..\..\..\src\net\SPClient\src\Player.h" //if static lib is fine, not sure how to properly include properly from are executable file. 
class SPNetServer : NonCopyable {

	class ClientConnect : public NetEngine 
	{

		#define  RECV_PACKET_CASE(T) \
		case Cmd##T: { \
			SPPacket_##T packet; \
			BinDeserializer de(data, hdr.len); \
			packet.io(de); \
			OnRecvPacket_##T(s, packet);\
		}break; \
		
		#define 	CmdHello 0
		#define		CmdMsg 1

		public:

		void onRecvPacket(std::unique_ptr<NESocket>& s, const NEPacketHeader& hdr, const u8* data) {
			switch (hdr.cmd) {
				RECV_PACKET_CASE(Hello)
			}
		}

		void OnRecvPacket_Hello(std::unique_ptr<NESocket>& s, SPPacket_Hello& packet)
		{
			SPPacket_Hello Packet;
			Packet.msg = "hi";
			sendPacket(s, Packet);
		}
		
		std::unique_ptr <SPNetServer>  _server = nullptr;
	};

	public:

	void RunServer();
	virtual void onInitServer();
	virtual void onDeinitServer();
	void update() ;

	void UpdateListenPoll();

	void StartListen(const SockAddr& addr);
	void StopListen();


	void sendToAll(std::unique_ptr<NESocket>& s, SPPacket& pkt) {
		for (auto& c : PlayerConnects) {
			c->sendPacket(s,pkt);
		}
	}

	private:
	bool isServerRunning = true;

	private:
	size_t _maxPacketSize = 4096;
	size_t _maxPacketPerFrame = 128;

	Vector<u8>		_recvPacketBuf;
	Vector< std::unique_ptr<NESocket> > _sockList;
	Vector< std::unique_ptr<Player> > PlayerList;
	Vector< std::unique_ptr<ClientConnect> >PlayerConnects;
	Vector< MyPollFD > _pollfds;

	Socket _listenSock;
	Socket _sock;
	String _sockBuf;
	SockAddr sockAddr;
	int port = 30000;
	char hostname[256] = "localhost";
};