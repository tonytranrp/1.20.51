#pragma once

#include "Block.h"
#include "Macro.h"
#include "ISubChunkStoragePaletted.h"

#include <gsl/span>

#include <cstdint>

class SubChunkBlockStorage
{
public:
    char pad_0008[1640]; //0x0008
    class N00002CBA *paletteData; //0x0670
    char pad_0678[56]; //0x0678
    int64_t paletteSize; //0x06B0
    char pad_06B8[784]; //0x06B8

    virtual ~SubChunkBlockStorage() = default;

	[[nodiscard]] bool isUniform(const Block* block) const
	{
		return vftCall<1, bool, const SubChunkBlockStorage*, const Block*>(this, block);
	}
	bool isPaletteUniform(const Block* block) const
	{
		return vftCall<2, const Block*, const SubChunkBlockStorage*, const Block*>(this, block);
	};
	[[nodiscard]] const Block* getElement(uint16_t index) const
	{
		return vftCall<3, const Block*, const SubChunkBlockStorage*, uint16_t>(this, index);
	}
	[[nodiscard]] size_t getSize() const
	{
		return vftCall<17, size_t, const SubChunkBlockStorage*>(this);
	}
	[[nodiscard]] const gsl::span<const Block*>& getPaletteSnapshot(gsl::span<const Block*>* out) const
	{
		return vftCall<18, const gsl::span<const Block*>&, const SubChunkBlockStorage*, gsl::span<const Block*>*>(this, out);
	}
};