#pragma once

#include <Math/Math.h>

#include "InputMode.h"
#include "Packet.h"

enum class AuthInputAction : uint64_t {
    ASCEND = 1ULL << 0,
    DESCEND = 1ULL << 1,
    NORTH_JUMP = 1ULL << 2,
    JUMP_DOWN = 1ULL << 3,
    SPRINT_DOWN = 1ULL << 4,
    CHANGE_HEIGHT = 1ULL << 5,
    JUMPING = 1ULL << 6,
    AUTO_JUMPING_IN_WATER = 1ULL << 7,
    SNEAKING = 1ULL << 8,
    SNEAK_DOWN = 1ULL << 9,
    UP = 1ULL << 10,
    DOWN = 1ULL << 11,
    LEFT = 1ULL << 12,
    RIGHT = 1ULL << 13,
    UP_LEFT = 1ULL << 14,
    UP_RIGHT = 1ULL << 15,
    WANT_UP = 1ULL << 16,
    WANT_DOWN = 1ULL << 17,
    WANT_DOWN_SLOW = 1ULL << 18,
    WANT_UP_SLOW = 1ULL << 19,
    SPRINTING = 1ULL << 20,
    ASCEND_SCAFFOLDING = 1ULL << 21,
    DESCEND_SCAFFOLDING = 1ULL << 22,
    SNEAK_TOGGLE_DOWN = 1ULL << 23,
    PERSIST_SNEAK = 1ULL << 24,
    START_SPRINTING = 1ULL << 25,
    STOP_SPRINTING = 1ULL << 26,
    START_SNEAKING = 1ULL << 27,
    STOP_SNEAKING = 1ULL << 28,
    START_SWIMMING = 1ULL << 29,
    STOP_SWIMMING = 1ULL << 30,
    START_JUMPING = 1ULL << 31,
    START_GLIDING = 1ULL << 32,
    STOP_GLIDING = 1ULL << 33,
    PERFORM_ITEM_INTERACTION = 1ULL << 34,
    PERFORM_BLOCK_ACTIONS = 1ULL << 35,
    PERFORM_ITEM_STACK_REQUEST = 1ULL << 36,
    HANDLE_TELEPORT = 1ULL << 37,
    EMOTING = 1ULL << 38
};

inline AuthInputAction operator|(AuthInputAction a, AuthInputAction b) {
    return static_cast<AuthInputAction>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline AuthInputAction operator&(AuthInputAction a, AuthInputAction b) {
    return static_cast<AuthInputAction>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

inline AuthInputAction operator^(AuthInputAction a, AuthInputAction b) {
    return static_cast<AuthInputAction>(static_cast<uint64_t>(a) ^ static_cast<uint64_t>(b));
}

inline AuthInputAction operator~(AuthInputAction a) {
    return static_cast<AuthInputAction>(~static_cast<uint64_t>(a));
}

inline void operator|=(AuthInputAction& a, AuthInputAction b) {
    a = a | b;
}

inline void operator&=(AuthInputAction& a, AuthInputAction b) {
    a = a & b;
}

inline void operator^=(AuthInputAction& a, AuthInputAction b) {
    a = a ^ b;
}

inline bool operator!(AuthInputAction a) {
    return static_cast<uint64_t>(a) == 0;
}

enum class ClientPlayMode {
    NORMAL,
    TEASER,
    SCREEN,
    VIEWER,
    REALITY,
    PLACEMENT,
    LIVING_ROOM,
    EXIT_LEVEL,
    EXIT_LEVEL_LIVING_ROOM
};

struct PlayerAuthInputPacket : public Packet {
	/* Fields */
	Vector2<float> rotation; //0x0030
	Vector3<float> position; //0x0038
	float headYaw; //0x0044
	char pad_0048[4]; //0x0048
	float downVelocity; //0x004C
    char pad_0050[32]; //0x0050
    AuthInputAction inputData; //0x0070
    InputMode inputMode; //0x0078
    ClientPlayMode playMode; //0x007C
    char pad_0080[8]; //0x0080
    int32_t ticksAlive; //0x0088
    char pad_008C[44]; //0x008C


	/* Virtuals */
	/* Functions */
    PlayerAuthInputPacket() {};
	PlayerAuthInputPacket(const PlayerAuthInputPacket& other) : Packet()
	{
		memcpy(this, &other, sizeof(PlayerAuthInputPacket));
	}
	void operator=(const PlayerAuthInputPacket& other)
	{
		memcpy(this, &other, sizeof(PlayerAuthInputPacket));
	}


};