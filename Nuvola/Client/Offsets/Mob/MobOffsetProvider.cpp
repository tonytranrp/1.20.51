#include "MobOffsetProvider.h"

MobOffsetProvider::MobOffsetProvider() : OffsetProvider("Mob")
{
	this->addResult<int32_t>(OffsetTable::Mob_mSwingProgress, "44 0f 2f 93 ? ? ? ? 73", 4);
	this->addResult<int32_t>(OffsetTable::Mob_mIsSwinging, "88 93 ? ? ? ? eb ? 33 d2", 2);
	this->addResult<int32_t>(OffsetTable::Mob_mSwingTicks, "89 93 ? ? ? ? 3b d0", 2);
	this->addResult<int32_t>(OffsetTable::Mob_mHurtTimeComponent, "48 8b 86 ? ? ? ? 48 89 6c 24 ? 48 85 c0", 3);
}
