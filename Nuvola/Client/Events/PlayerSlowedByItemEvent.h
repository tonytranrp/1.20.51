#pragma once

#include "Cancellable.h"
#include "PlayerEvent.h"

class PlayerSlowedByItemEvent : public PlayerEvent, public Cancellable
{
public:
	PlayerSlowedByItemEvent(Player* player) : PlayerEvent(player)
	{

	}
};