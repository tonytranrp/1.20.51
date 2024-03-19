#pragma once

#include "ActorEvent.h"
#include "Cancellable.h"

class ActorMoveEvent : public ActorEvent, public Cancellable {
	Vector3<float>* position;
public:
	ActorMoveEvent(Actor* actor, Vector3<float>* position) : ActorEvent(actor) {
		this->position = position;
	};

	const Vector3<float>& getPosition() {
		return *this->position;
	}
	void setPosition(Vector3<float> position) {
		*this->position = position;
	}
};