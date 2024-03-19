#include "LocalPlayerOffsetProvider.h"

LocalPlayerOffsetProvider::LocalPlayerOffsetProvider() : OffsetProvider("LocalPlayer")
{
	this->addResult(OffsetTable::LocalPlayer_LocalTicksAlive, 0x1F30);
}
