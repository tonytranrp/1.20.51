#pragma once

#include "../IPatch.h"

#include "../../Bridge/Actor.h"
#include "../../Bridge/Block.h"
#include "../../Bridge/BlockLegacy.h"
#include "../../Bridge/BlockSource.h"
#include "../../Bridge/Types.h"

#include <Math/Math.h>

class FireBlockChangedHook : public IPatch
{
	static void __fastcall fireBlockChangedCallback_1_20_12_1(BlockSource* self, const BlockPos& blockPos,
															  uint32_t param_2, const Block* oldBlock,
															  const Block* newBlock, int param_5,
															  int32_t eventTarget,
															  class ActorBlockSyncMessage* param_7,
															  Actor* who);
public:
	static inline uintptr_t funcOriginal = 0;
	FireBlockChangedHook();
	auto apply() -> bool override;
};