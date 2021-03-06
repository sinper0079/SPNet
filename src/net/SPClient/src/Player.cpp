#include "Player.h"

namespace sita {

void Player::draw() {
	if (!_active) return;

	auto* drawList = ImGui::GetBackgroundDrawList();

	drawList->AddTriangleFilled(ImVec2(_pos.x, _pos.y - 12),
		ImVec2(_pos.x - 12, _pos.y + 12),
		ImVec2(_pos.x + 12, _pos.y + 12),
		_color);
}

void Player::move(const ImVec2& v) {
	_pos += v;
}

} // namespace