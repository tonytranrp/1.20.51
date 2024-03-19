#pragma once

#include "Cancellable.h"
#include "RakEvent.h"
#include "../Bridge/RakNet/RakPeer.h"

class RakPeerRunUpdateCycleEvent : public RakEvent, public Cancellable
{
private:
	RakNet::RakPeer* peer;
public:
	RakPeerRunUpdateCycleEvent(RakNet::RakPeer* peer)
	{
		this->peer = peer;
	}
	
	RakNet::RakPeer* getRakPeer()
	{
		return this->peer;
	}
};