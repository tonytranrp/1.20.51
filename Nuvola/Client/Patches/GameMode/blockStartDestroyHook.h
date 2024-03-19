#pragma once

#include "../IPatch.h"

#include "../../Bridge/Dimension.h"
#include "../../Bridge/GameMode.h"
#include "../../Bridge/Types.h"
#include "../../Bridge/Actor.h"
#include <Math/Math.h>


class blockStartDestroyHook : public IPatch
{
	static bool __fastcall blockStartDestroyCallback_1_20_15(GameMode* _this, BlockPos* blockPos, unsigned char a3, bool* a4);
public:
	static inline uintptr_t funcOriginal = 0;
	blockStartDestroyHook();
	auto apply() -> bool override;
};