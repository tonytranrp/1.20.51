#pragma once

#include "../Signatures/SigManager.h"
#include "ActorRuntimeID.h"
#include "Packet.h"

#include <Math/Math.h>

#include <cstdint>

enum class InteractAction
{
	RIGHT_CLICK = 1,
	LEFT_CLICK = 2,
	LEAVE_VEHICLE = 3,
	MOUSEOVER = 4
};

class InteractPacket : public Packet
{
public:
	int8_t action{}; //0x0030
	char pad_0031[7]{}; //0x0031
	int32_t actorRuntimeID{}; //0x0038
	char pad_003C[4]{}; //0x003C
	Vector3<float> interactPos; //0x0040
	char pad_004C[4]{}; //0x004C

	InteractPacket(InteractAction action, ActorRuntimeID actorRuntimeID, Vector3<float> interactPos) : Packet()
	{
		auto func = systems->getSigManager()->findAddressAs<InteractPacket *(__thiscall *)(InteractPacket *, InteractAction,
                                                                             ActorRuntimeID, Vector3<float> &)>(
                SigTable::InteractPacket_cctor);
		func(this, action, actorRuntimeID, interactPos);
	}
};
static_assert(sizeof(InteractPacket) == 0x50);