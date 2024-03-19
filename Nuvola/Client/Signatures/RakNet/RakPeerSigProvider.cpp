#include "RakPeerSigProvider.h"

#include <xorstr.hpp>

RakPeerSigProvider::RakPeerSigProvider() : SigProvider("RakPeer")
{
	this->addResult(SigTable::RakNet_RakPeer_RunUpdateCycle, std::string("48 89 5c 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? b8 ? ? ? ? e8 ? ? ? ? 48 2b e0 0f 29 b4 24 ? ? ? ? 0f 29 bc 24 ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 8b fa 48 89 55 ? 4c 8b e1"));
}
