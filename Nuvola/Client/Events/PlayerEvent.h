#ifndef NUVOLA_CLIENT_EVENTS_PLAYEREVENT
#define NUVOLA_CLIENT_EVENTS_PLAYEREVENT

#include "Event.h"
#include "../Bridge/Player.h"

class PlayerEvent : public Event {
	Player* player;
public:
	PlayerEvent(Player* player) : Event() {
		this->player = player;
	};

	Player* getPlayer() {
		return this->player;
	}
};

#endif /* NUVOLA_CLIENT_EVENTS_PLAYEREVENT */
