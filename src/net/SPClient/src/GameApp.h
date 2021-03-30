#include "..\..\examples\sita_net_example_002\src\GameApp.h"
#pragma once

#include "pch.h"
namespace sita {

class GameApp {
public:
	virtual ~GameApp();

	bool getInputKey(SDL_Keycode key) {
		auto s = SDL_GetScancodeFromKey(key);
		return ImGui::GetIO().KeysDown[s];
	}

	void run();


	virtual void onUpdate(float deltaTime) {}
	virtual void onEvent(SDL_Event& ev) {}
	std::unique_ptr<NetworkComponent>NetComponent;
	ImVec2 screenSize() const;

	virtual void onInit() {
		NetComponent = std::make_unique<NetworkComponent>();
		NetComponent->connect(NetComponent->hostname, NetComponent->port);
	}
	virtual void onDeinit() {
		NetComponent.reset();
	}
};


} // namespace