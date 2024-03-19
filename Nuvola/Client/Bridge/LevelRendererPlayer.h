#pragma once

#include <Math/Math.h>

class LevelRendererPlayer
{
	Sizer<0x60C> size;
public:
	DEFINE_MEMBER(Vector3<float>, mOrigin, OffsetTable::LevelRendererPlayer_mOrigin);
	DEFINE_MEMBER(Vector3<float>, mLastOrigin, OffsetTable::LevelRendererPlayer_mLastOrigin);
}; //Size: 0x05FC
static_assert(sizeof(LevelRendererPlayer) == 0x60C);