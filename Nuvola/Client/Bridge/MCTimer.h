#pragma once

class MCTimer
{
public:
	float ticksPerSecond; //0x0000
	char pad_0004[4]; //0x0004
	float tickDelta; //0x0008
	float tickSpeedMultiplier; //0x000C
	float lastTickDelta; //0x0010
	float N00002794; //0x0014
	float seconds; //0x0018
	float frameDelta; //0x001C
	char pad_0020[16]; //0x0020
}; //Size: 0x0030