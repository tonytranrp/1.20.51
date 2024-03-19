#pragma once

#include "ActorEvent.h"
#include "Cancellable.h"

class ActorMotionEvent : public ActorEvent, public Cancellable {
	Vector3<float>* motion;
public:
	ActorMotionEvent(Actor* actor, Vector3<float>* motion) : ActorEvent(actor) {
		this->motion = motion;
	};

	Vector3<float>* getMotion() {
		return this->motion;
	};
	void setMotion(Vector3<float> motion) {
		*this->motion = motion;
	};
};