#pragma once
#include "Socket.h"

namespace sita{
#ifdef _WIN32
typedef WSAPOLLFD MyPollFD_Base;
#else
typedef struct pollfd MyPollFD_Base;
#endif


struct MyPollFD : MyPollFD_Base {
	using Base = MyPollFD_Base;

	MyPollFD() {
		fd = Socket::kInvalidHandle;
		events = 0;
		revents = 0;
	}

	void reset(Socket& sock, bool read, bool write) {
		fd = sock.sock();
		revents = 0;
		events = 0;
		if (read) events |= POLLIN;
		if (write) events |= POLLOUT;
	}

	bool canRead() { return revents & POLLIN; }
	bool canWrite() { return revents & POLLOUT; }
};



#ifdef _WIN32
inline int my_poll(MyPollFD* pollfd, size_t pollfdCount, int timeoutMillisecond) {
	int ret = WSAPoll(pollfd, pollfdCount, timeoutMillisecond);
	if (ret < 0) {
		throw SITA_ERROR("poll");
	}
	return ret;
}

#else

inline int my_poll(MyPollFD* pollfd, size_t pollfdCount, int timeoutMillisecond) {
	int ret = ::poll(pollfd, pollfdCount, timeoutMillisecond);
	if (ret < 0)
		throw SITA_ERROR("poll");
	return ret;
}
#endif




	class SPSocket : Socket
	{






	};


} //sita namespace