#pragma once

#include "ActorEvent.h"
#include "BlockLegacyEvent.h"

class BlockChangedEvent : public ActorEvent, public BlockLegacyEvent
{
	BlockPos where;
	BlockLegacy* oldBlock;
public:
	BlockChangedEvent(BlockPos where, BlockLegacy* oldBlock, BlockLegacy* newBlock, Actor* who) : ActorEvent(who), BlockLegacyEvent(newBlock), where(where), oldBlock(oldBlock) {};

	[[nodiscard]] const BlockPos& getWhere() const
	{
		return this->where;
	}
	[[nodiscard]] const BlockLegacy* getOldBlock() const
	{
		return this->oldBlock;
	}
};