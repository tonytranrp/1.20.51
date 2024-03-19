#include "BlockLegacySigProvider.h"

BlockLegacySigProvider::BlockLegacySigProvider() : SigProvider("BlockLegacy")
{
	this->addResult(SigTable::BlockLegacy_getRenderLayer, "8B 81 04 01 00 00 C3");
}