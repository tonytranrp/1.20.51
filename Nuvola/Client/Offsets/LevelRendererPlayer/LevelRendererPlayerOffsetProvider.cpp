#include "LevelRendererPlayerOffsetProvider.h"

LevelRendererPlayerOffsetProvider::LevelRendererPlayerOffsetProvider() : OffsetProvider("LevelRendererPlayer")
{
	this->addResult<int32_t>(OffsetTable::LevelRendererPlayer_mOrigin, "f3 0f 10 a0 ? ? ? ? 0f 10 80", 4);
	this->addResult<int32_t>(OffsetTable::LevelRendererPlayer_mLastOrigin, "f3 0f 10 8f ? ? ? ? 4c 63 73", 4);
}
