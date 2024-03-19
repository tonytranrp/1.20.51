#pragma once

#include "Event.h"
#include "../Bridge/Actor.h"

class ActorEvent : public Event {
	Actor* actor;
public:
	ActorEvent(Actor* actor) : Event() {
		this->actor = actor;
	};

	auto getActor() -> Actor* {
		return this->actor;
	};
};
