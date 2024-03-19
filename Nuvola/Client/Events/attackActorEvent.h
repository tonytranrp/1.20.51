#pragma once

#include "../Bridge/Actor.h"
#include "../Bridge/GameMode.h"

#include "Event.h"

class AttackActorEvent : public Event
{
	Actor* actor;
	GameMode* gamemode;
public:
	AttackActorEvent(GameMode* gamemode, Actor* actor) {
		this->actor = actor;
		this->gamemode = gamemode;
	}

	Actor* getActor() {
		return this->actor;
	}

	GameMode* getGameMode() {
		return this->gamemode;
	}
};