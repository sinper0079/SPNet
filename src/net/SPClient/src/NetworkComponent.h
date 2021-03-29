#pragma once

#include"pch.h"

namespace sita {
	class NetworkComponent : public NonCopyable 
	{
	public :
		void connect(const char* hostname, uint16_t port);

		virtual void onConnected() {}
		virtual void onRecvPacket(SPPacketType packetType, const Vector<char>& buf) {}
		virtual void onRecvPacket(SPPacketType packetType, const Vector<u8>& buf) {}
		void acceptFromListenSock(Socket& listenSock);

		void sendPacket(SPPacket& pkt);

		enum class Status {
			None,
			Connecting,
			Connected,
			Closed,
		};

		void onUpdate(MyPollFD& fd);
		void getPollFD(MyPollFD& pf);

		void close();
		bool isValid();

		int port = 30000;
		char hostname[256] = "localhost";

	private:
		void _onConnected();
		void _onRecv();
		void _onSend();

		Status _status = Status::None;

		MyPollFD _pollfd;

		Vector<char> _tmp;
		
		Vector<u8>_recvBuf;

		Vector<char> _recvBufChar; // cant overload variable?
		Vector<u8> _sendBuf;
		Vector<char> _sendBufChar;
		size_t _sendBufOffset = 0;
		Socket _sock;



	};
}


