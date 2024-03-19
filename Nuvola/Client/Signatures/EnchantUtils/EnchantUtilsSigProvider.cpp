#include "EnchantUtilsSigProvider.h"

#include "../../../Mem/Mem.h"

#include <xorstr.hpp>

EnchantUtilsSigProvider::EnchantUtilsSigProvider() : SigProvider("EnchantUtils")
{
	this->addResult(SigTable::EnchantUtils_getEnchantLevel, std::string("48 89 5c 24 ?? 48 89 74 24 ?? 57 48 83 ec ?? 48 8b f2 0f b6 d9"));
	this->addResult(SigTable::EnchantUtils_getEnchantNameAndLevel, std::string("40 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 41 8B F8"));
}
