#pragma once
#include "pch.h"

class SPNetServer : NetEngine {
	public:

	void RunServer();
	virtual void onInitServer();
	virtual void onDeinitServer();
	void _update();

	private:
	bool isServerRunning = true;


};