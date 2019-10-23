#include "Body.h"
#include "Interaction.h"

bool Body::checkInteraction(int _bodyId) {
	for (auto& I : inter) {
		auto Il = I.lock();

		auto Ilb1 = Il->body1.lock();
		auto Ilb2 = Il->body2.lock();

		if ((Ilb1->id == id && Ilb2->id == _bodyId) || (Ilb2->id == id && Ilb1->id == _bodyId))  return true;
	}
	return false;
}