#pragma once

#include "Player.h"

#include <cstdint>

struct LocalPlayer : public Player {
	DEFINE_MEMBER(int32_t, localTicksAlive, OffsetTable::LocalPlayer_LocalTicksAlive);
	/* Fields */
	//int32_t localTicksAlive; //0x1EF0
	//char pad_1EF4[176]; //0x1EF4
	/* Virtuals */
	/* Functions */
};
