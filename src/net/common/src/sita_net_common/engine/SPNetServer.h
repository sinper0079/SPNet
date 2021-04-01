#pragma once
#include "..\..\..\src\net\SPClient\src\Player.h" //if static lib is fine, not sure how to properly include properly from are executable file. 
class SPNetServer : NetEngine {

	class ClientConnect : public NetworkComponent {
	public:
		virtual void onRecvPacket(SPPacketType packetType, const std::vector<u8>& buf) override {
			switch (packetType) {
			//case SPPacketType::Hello: {
			//	SPPacket_Hello pkt;
			//	pkt.fromBuffer(buf);
			//}break;

			//case SPPacketType::Chat: {
			//	SPPacket_Chat pkt;
			//	pkt.fromBuffer(buf);
			//	printf("chat: \"%s\"\n", pkt.msg.c_str());
			//	for (auto& t : pkt.toUsers) {
			//		printf("  to: \"%s\"\n", t.c_str());
			//	}

			//	// toupper and send to all
			//	for (auto& ch : pkt.msg) {
			//		ch = toupper(ch);
			//	}

			//	_server->sendToAll(pkt);
			//}break;
			}
		}

		SPNetServer* _server = nullptr;
	};


#define  RECV_PACKET_CASE(T) \
		case Cmd##T: { \
			SPPacket_##T packet; \
			BinDeserializer de(data, hdr.len); \
			de.io(packet); \
		}break; \



#define 	CmdHello 0
#define		CmdMsg 1

	public:
		 void onRecvPacket(std::unique_ptr<NESocket>& s, const NEPacketHeader& hdr, const u8* data) {
			switch (hdr.cmd) {
				RECV_PACKET_CASE(Hello)



			}
		}
	void RunServer();
	virtual void onInitServer();
	virtual void onDeinitServer();
	virtual void update() override;

	void UpdateListenPoll();

	void StartListen(const SockAddr& addr);
	void StopListen();

	virtual void onRecv(std::unique_ptr<NESocket>& s) ;

	void sendToAll(SPPacket& pkt) {
		for (auto& c : PlayerConnects) {
			c->sendPacket(pkt);
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