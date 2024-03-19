#include "LevelOffsetProvider.h"

LevelOffsetProvider::LevelOffsetProvider() : OffsetProvider("Level")
{
	this->addResult(OffsetTable::Level_mActorFactory, 0x17F8);
	this->addResult(OffsetTable::Level_mActorManager, 0x1F30);
	this->addResult<int32_t>(OffsetTable::Level_mBlockPalette, "48 8b 88 ? ? ? ? 48 8b 01 48 8b 40 ? ff 15 ? ? ? ? 4c 8b 4b", 3);
	this->addResult(OffsetTable::Level_mHitResult, 0xA48);
}
