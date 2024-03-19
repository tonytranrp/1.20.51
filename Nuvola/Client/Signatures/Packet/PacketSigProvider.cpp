#include "PacketSigProvider.h"

PacketSigProvider::PacketSigProvider() : SigProvider("Packet")
{
	this->addResult(SigTable::MovePlayerPacket_cctor, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 4C 24 ? 56 57 41 56 48 83 EC ? 49 8B E8 48 8B F2 48 8B D9");
	this->addResult(SigTable::InteractPacket_cctor, "48 89 4c 24 ? 53 48 83 ec ? 48 8b d9 c7 41 ? ? ? ? ? c7 41 ? ? ? ? ? 66 c7 41 ? ? ? 33 c0 48 89 41 ? 48 89 41 ? 89 41 ? 48 8d 05 ? ? ? ? 48 89 01 88 51");
}