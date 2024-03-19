#pragma once

#include "../Signatures/SigManager.h"

#include "Enchant.h"
#include "ItemStackBase.h"

#include <string>
#include <vector>

namespace EnchantUtils
{
	inline static int getEnchantLevel(EnchantType type, const ItemStackBase& stack)
	{
		auto func = (int(__cdecl*)(EnchantType, const ItemStackBase&)) systems->getSigManager()->findAddressOf(
                SigTable::EnchantUtils_getEnchantLevel);
		return func(type, stack);
	}

	inline static std::string getEnchantNameAndLevel(EnchantType type, int level)
	{
		auto func = systems->getSigManager()->findAddressAs<std::string(__cdecl *)(EnchantType, int)>(
                SigTable::EnchantUtils_getEnchantNameAndLevel);
		return func(type, level);
	}
}