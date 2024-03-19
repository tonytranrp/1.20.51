#pragma once

#include <Math/Math.h>
#include "../../Mem/Mem.h"

#include "EntityContext.h"
#include "LevelRendererPlayer.h"

class LevelRenderer
{
public:
	char pad_0000[224]; //0x0000
	class N000024FD *N000010A9; //0x00E0
	char pad_00E8[88]; //0x00E8
	class N00004C22 *N000010B5; //0x0140
	char pad_0148[208]; //0x0148
	int32_t mTicks; //0x0218
	char pad_021C[84]; //0x021C
	class N00002659 *mTaskGroup; //0x0270
	class N00002645 *mSyncTaskGroup; //0x0278
	class GameRenderer *mGameRenderer; //0x0280
	class Level *mLevel; //0x0288
	char pad_0290[8]; //0x0290
	class Options *mOptions; //0x0298
	class ClientInstance *mClientInstance; //0x02A0
	char pad_02A8[8]; //0x02A8
	EntityContext mLocalPlayerContext; //0x02B0
	char pad_02C0[64]; //0x02C0
	class HolographicPlatform *mHolographicPlatform; //0x0300
	LevelRendererPlayer* levelRendererPlayer; //0x0308
}; //Size: 0x0810