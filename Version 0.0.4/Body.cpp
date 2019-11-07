#include "Body.h"
#include "Interaction.h"

bool Body::checkInteraction(int _bodyId) {
	for (auto& I : inter) {
		auto Il = I.lock();

		auto Ilb1 = Il->body1.lock();
		auto Ilb2 = Il->body2.lock();

		if ((Ilb1->id == id && Ilb2->id == _bodyId) || (Ilb2->id == id && Ilb1->id == _bodyId)) {
			return true;
		}
	}
	return false;
}

bool Body::fluidInteraction(Vec2d _cellPos, double _dx) {
	Vec2d Delta;
	Delta[0] = pos[0] - std::max(_cellPos[0], std::min(pos[0], _cellPos[0] + _dx));
	Delta[1] = pos[1] - std::max(_cellPos[1], std::min(pos[1], _cellPos[1] + _dx));
	if (Delta.dot(Delta) < radius * radius) {
		return true;
	}
	return false;
}