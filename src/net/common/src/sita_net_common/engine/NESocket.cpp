#include "NESocket.h"

void sita::NESocket::close()
{
	_sock.close();
	SITA_LOG("Close socket."); 
}
