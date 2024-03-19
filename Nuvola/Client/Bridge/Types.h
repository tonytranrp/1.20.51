#pragma once

#include <Math/Math.h>

typedef Vector3<int> BlockPos;
class ChunkPos : public Vector2<int>
{
public:
	ChunkPos() : Vector2<int>(0, 0) {};
	ChunkPos(int x, int z) : Vector2<int>(x, z) {}
	ChunkPos(const BlockPos& blockPos)
	{
		this->x = blockPos.x >> 4;
		this->y = blockPos.z >> 4;
	}

	bool operator==(const ChunkPos& other) const
	{
		return this->x == other.x && this->y == other.y;
	}
};
template<>
struct std::hash<ChunkPos>
{
	size_t operator()(const ChunkPos& obj) const {
		return obj.hash();
	}
};
class ChunkBlockPos : public Vector3<int>
{
public:
	explicit ChunkBlockPos(const BlockPos& blockPos)
	{
		this->x = blockPos.x % 16;
		this->y = blockPos.y + 64;
		this->z = blockPos.z % 16;
	}
};