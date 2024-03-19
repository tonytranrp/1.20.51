#include "EnchantSigProvider.h"

#include "../../../Mem/Mem.h"

#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

EnchantSigProvider::EnchantSigProvider() : SigProvider("Enchant")
{
	uintptr_t enchantsSig = Mem::FindSig(" ");
	assert(enchantsSig != 0);
	uintptr_t enchantsOffsetRef = enchantsSig + 3;
	uint32_t enchantsOffset = *reinterpret_cast<uint32_t*>(enchantsOffsetRef);
	float* enchantsPtr = reinterpret_cast<float*>(enchantsOffsetRef + enchantsOffset + 4);

	//Unprotect the memory values
	DWORD oldProt = 0;
	VirtualProtect(enchantsPtr, 4, PAGE_EXECUTE_READWRITE, &oldProt);

	this->addResult(SigTable::Enchant_mEnchants, enchantsPtr);
}
