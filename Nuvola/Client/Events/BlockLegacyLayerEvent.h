#pragma once

#include "../Bridge/Types.h"

#include "BlockLegacyEvent.h"

class BlockLegacyLayerEvent : public BlockLegacyEvent
{
	BlockRenderLayer layer;
public:
	BlockLegacyLayerEvent(const BlockLegacy* block, BlockRenderLayer layer) : BlockLegacyEvent(block), layer(layer) {};

	BlockRenderLayer getLayer() const
	{
		return this->layer;
	}

	void setLayer(BlockRenderLayer layer)
	{
		this->layer = layer;
	}
};