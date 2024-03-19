#pragma once

#include "../Bridge/BlockLegacy.h"

#include "Event.h"

class BlockLegacyEvent : public Event
{
	const BlockLegacy* block = nullptr;
public:
	BlockLegacyEvent(const BlockLegacy* block) : Event(), block(block) {};

	const BlockLegacy* getBlock() const
	{
		return this->block;
	}
};