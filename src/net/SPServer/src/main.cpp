#include "pch.h"
#include "SPNetServer.h"

int main()
{
	SITA_LOG("Server Start");
	SPNetServer	SpNetServer;
	SpNetServer.RunServer();
}