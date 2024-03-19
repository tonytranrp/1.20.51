#pragma once

#include "../Signatures/SigManager.h"

#include "Packet.h"

#include <memory>

namespace MinecraftPackets
{
	static std::shared_ptr<Packet> __cdecl createPacket(PacketID id)
	{
		auto func = systems->getSigManager()->findAddressAs<std::shared_ptr<Packet> (__cdecl *)(PacketID)>(
                SigTable::MinecraftPackets_createPacket);
		return func(id);
	}
}