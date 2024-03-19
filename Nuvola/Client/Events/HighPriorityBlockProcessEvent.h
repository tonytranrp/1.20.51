#pragma once

#include "BlockLegacyEvent.h"

class HighPriorityBlockProcessEvent : public BlockLegacyEvent
{
	Vector3<int> pos;
public:
	HighPriorityBlockProcessEvent(const BlockLegacy* block, const Vector3<int>& pos) : BlockLegacyEvent(block), pos(pos) {}

	const Vector3<int>& getPos() const
	{
		return this->pos;
	}
};