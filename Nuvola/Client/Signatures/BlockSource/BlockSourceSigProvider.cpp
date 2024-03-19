#include "BlockSourceSigProvider.h"

#include "../../../Mem/Mem.h"

#include <cassert>

BlockSourceSigProvider::BlockSourceSigProvider() : SigProvider("BlockSource")
{
    /*intptr_t vtableOffsetSig = Mem::FindSig("48 8d 05 ? ? ? ? 48 89 85 ? ? ? ? 41 8b 06");
	assert(vtableOffsetSig != 0);
    uintptr_t vtableOffsetRef = vtableOffsetSig + 3;
    int32_t vtableOffset = *reinterpret_cast<int32_t*>(vtableOffsetRef);
    uintptr_t vtablePtr = vtableOffsetRef + vtableOffset + sizeof(vtableOffset);
    this->addResult(SigTable::BlockSource_vftable, reinterpret_cast<void*>(vtablePtr));

    uintptr_t getBlockOffsetSig = Mem::FindSig("48 8b 40 ? ff 15 ? ? ? ? 48 8b f0 0f 10 05 ? ? ? ? 0f 11 85");
	assert(getBlockOffsetSig != 0);
    uintptr_t getBlockOffsetRef = getBlockOffsetSig + 3;
    uint8_t getBlockOffset = *reinterpret_cast<uint8_t*>(getBlockOffsetRef);
    uintptr_t getBlockPtr = vtablePtr + getBlockOffset;
    this->addResult(SigTable::BlockSource_getBlock, *reinterpret_cast<void**>(getBlockPtr));*/

	this->addResult(SigTable::BlockSource_fireBlockChanged, "4c 8b dc 45 89 43 ? 49 89 53");
}