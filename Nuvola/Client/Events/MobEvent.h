#ifndef NUVOLA_CLIENT_EVENTS_MOBEVENT
#define NUVOLA_CLIENT_EVENTS_MOBEVENT

#include "../Bridge/Mob.h"
#include "Event.h"

class MobEvent : public Event {
	Mob* mob;
public:
	MobEvent(Mob* mob) : Event() {
		this->mob = mob;
	};

	Mob* getMob() {
		return this->mob;
	};
};

#endif /* NUVOLA_CLIENT_EVENTS_MOBEVENT */
