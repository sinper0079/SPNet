#pragma once
#include <sita_core.h>

struct GameRoom {

	int32_t MaxPlayer; 


};

namespace sita {

	class GameSession : NonCopyable {

	public:
		void CreateGameSession();
		void JoinGameSession();
		void OnConnection();
		void OnDisconnect();


	};

}
