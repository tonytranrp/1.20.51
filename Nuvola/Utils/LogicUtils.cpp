#include "LogicUtils.h"

/* Minecraft math shit */
float LogicUtils::vertJumpSpeed(int tick)
{
	if (tick <= 1)
	{
		return MC_JUMP_UPWARDS_MOTION;
	}
	return (vertJumpSpeed(tick - 1) - MC_GRAVITY) * MC_DRAG;
}

float LogicUtils::vertHeightAt(int tick)
{
	if (tick == 0) {
		return 0.0f;
	}
	return vertHeightAt(tick - 1) + vertJumpSpeed(tick);
}
