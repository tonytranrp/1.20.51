#pragma once

#include <Math/Math.h>

#include "../Signatures/SigManager.h"

#include "Packet.h"

enum MovePacketMode {
	MOVE_PACKET_MODE_NORMAL = 0,
	_RESET = 1,
	TELEPORT = 2,
	ROTATION = 3
};

struct MovePlayerPacket : public Packet {
	/* Fields */
	int64_t actorRuntimeID;
	Vector3<float> position;
	Vector2<float> rotation;
	float headYaw;
    uint8_t mode;
	bool onGround;
	uint64_t ridingRuntimeId;
	int teleportCause;
	__int32 teleportItem;
    __int32 tick;

	/* Virtuals */
	/* Functions */
	MovePlayerPacket(class Player* player, Vector3<float> position) : Packet()
	{
		auto cctor = systems->getSigManager()->findAddressAs<void(__thiscall*)(MovePlayerPacket*, class Player*, Vector3<float>*)>(
                SigTable::MovePlayerPacket_cctor);
		cctor(this, player, &position);
	}
	MovePlayerPacket(const MovePlayerPacket& other)
	{
		memcpy(this, &other, sizeof(MovePlayerPacket));
	}
	void operator=(const MovePlayerPacket& other)
	{
		memcpy(this, &other, sizeof(MovePlayerPacket));
	}
};