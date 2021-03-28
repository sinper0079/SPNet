#include "NESocket.h"

NESocketAddr::NESocketAddr() {
	memset(&_addr, 0, sizeof(_addr));
}

void NESocketAddr::setPort(uint16_t port) {
	*reinterpret_cast<uint16_t*>(_addr.sa_data) = htons(port);
}

uint16_t NESocketAddr::port() const {
	return ntohs(*reinterpret_cast<const uint16_t*>(_addr.sa_data));
}

void NESocketAddr::setIPv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	_addr.sa_family = AF_INET;
	_addr.sa_data[2] = a;
	_addr.sa_data[3] = b;
	_addr.sa_data[4] = c;
	_addr.sa_data[5] = d;
}

void NESocketAddr::setIPv4(const char* hostname, const char* service) {
	std::vector<NESocketAddr> addrs;
	myGetHostByNameIPv4(addrs, hostname, service);
	if (!addrs.size())
		throw SITA_ERROR ("setIPv4");

	*this = addrs[0];
}

void NESocketAddr::print() const {
	if (_addr.sa_family == AF_INET) {
		auto* p = reinterpret_cast<const uint8_t*>(_addr.sa_data);
		printf("IPv4 %u.%u.%u.%u : %u\n", p[2], p[3], p[4], p[5], port());//how to use fmf to change this?
	}
}



class NESocketInit {
public:
#ifdef _WIN32
	NESocketInit() {
		WSADATA data;
		if (0 != WSAStartup(MAKEWORD(2, 2), &data)) {
			throw SITA_ERROR("WSAStartup");
		}
	}

	~NESocketInit() {
		WSACleanup();
	}

	static void init() {
		static NESocketInit s;
	}
#else
	static void init() {
	}
#endif
};

void myGetHostByNameIPv4(std::vector<NESocketAddr>& outAddrs, const char* hostname, const char* service /*= nullptr*/) {
	outAddrs.clear();

	NESocketInit::init();

	// The gethostbyname function has been deprecated 
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	struct addrinfo* result = nullptr;

	if (0 != getaddrinfo(hostname, service, &hints, &result)) {
		throw SITA_ERROR("getaddrinfo");
	}

	if (!result)
		return;

	for (auto* r = result; r; r = r->ai_next) {
		outAddrs.emplace_back();
		outAddrs.back()._addr = *r->ai_addr;
	}

	freeaddrinfo(result);
}