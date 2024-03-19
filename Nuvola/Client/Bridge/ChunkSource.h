#pragma once

#include "Dimension.h"
#include "Level.h"
#include "LevelChunk.h"
#include "Macro.h"
#include "Types.h"

#include <Math/Math.h>

#include <memory>
#include <stdint.h>
#include <unordered_map>

class ChunkSource
{
	char pad_0008[24]; //0x0008
	Level* level; //0x0020
	Dimension* dimension; //0x0028
	char pad_0030[48]; //0x0030
	std::unordered_map<ChunkPos, std::weak_ptr<LevelChunk>> chunkStorage; //0x0060
public:
	enum LoadMode : int32_t {
		None = 0,
		Deferred = 1
	};

	/* Virtuals */
	virtual ~ChunkSource() = default;
	[[nodiscard]] bool isChunkKnown(const ChunkPos& chunkPos) const
	{
		return vftCall<5, bool, const ChunkSource*, const ChunkPos&>(this, chunkPos);
	}

	[[nodiscard]] std::shared_ptr<LevelChunk> getOrLoadChunk(const ChunkPos& chunkPos, LoadMode mode, bool param_3) const
	{
		return vftCall<8, std::shared_ptr<LevelChunk>, const ChunkSource*, const ChunkPos&, LoadMode, bool>(this, chunkPos, mode, param_3);
	}

	[[nodiscard]] const std::unordered_map<ChunkPos, std::weak_ptr<LevelChunk>>& getStorage() const
	{
		return vftCall<24, const std::unordered_map<ChunkPos, std::weak_ptr<LevelChunk>>&, const ChunkSource*>(this);
	}

	/* Functions */
	[[nodiscard]] inline bool isChunkLoaded(const ChunkPos& pos) const
	{
        for(const auto& [chunkPos, chunk] : this->chunkStorage)
        {
            if(pos == chunkPos)
            {
                return true;
            }
        }
        return false;
		//return chunkStorage.contains(pos);
	}
	[[nodiscard]] Level* getLevel() const
	{
		return this->level;
	}
	[[nodiscard]] Dimension* getDimension() const
	{
		return this->dimension;
	}
	std::weak_ptr<LevelChunk>& getChunkFor(BlockPos blockPos) const
	{
		auto storage = this->getStorage();
		return storage.at(ChunkPos(blockPos));
	}
};