#include "BlockSigProvider.h"

BlockSigProvider::BlockSigProvider() : SigProvider("Block")
{
	this->addResult(SigTable::Block_getRenderLayer, "40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 75 32");
}