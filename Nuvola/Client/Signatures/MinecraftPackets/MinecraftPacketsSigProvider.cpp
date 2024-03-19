#include "MinecraftPacketsSigProvider.h"

MinecraftPacketsSigProvider::MinecraftPacketsSigProvider() : SigProvider("MinecraftPackets")
{
	this->addResult(SigTable::MinecraftPackets_createPacket, "40 53 48 83 ec ? 45 33 c0 48 8b d9 ff ca 81 fa");
}