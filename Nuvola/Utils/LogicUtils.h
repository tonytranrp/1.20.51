#pragma once

#define MC_JUMP_UPWARDS_MOTION 0.42f
#define MC_JUMP_TICKS 12
#define MC_GRAVITY 0.08f
#define MC_DRAG 0.98f;
#define MC_TERMINAL_VELOCITY -3.92f;

namespace LogicUtils
{
	float vertJumpSpeed(int tick);
	float vertHeightAt(int tick);
}