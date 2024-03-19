#pragma once

#include <optional>
#include <string>
#include <stdint.h>

#include "ItemStackBase.h"

#include "../Signatures/SigManager.h"

#pragma pack(push, 1)
class ItemStack : public ItemStackBase
{
public:
	uint8_t stackNetId = 0; //0x0088
	char pad_0089[23]; //0x0089

	ItemStack()
	{
		memset(this, 0, sizeof(ItemStack));
	}

	ItemStack(const ItemStack& other)
	{
		auto func = (ItemStackBase & (__thiscall*)(ItemStackBase*, const ItemStackBase&)) systems->getSigManager()->findAddressOf(SigTable::ItemStackBase_cctor);
		func(this, other);
		this->stackNetId = other.stackNetId;
	}

	ItemStack* operator=(const ItemStack& other) {
		auto func = (ItemStackBase & (__thiscall*)(ItemStackBase*, const ItemStackBase&)) systems->getSigManager()->findAddressOf(SigTable::ItemStackBaseOperatorEquals);
		func(this, other);
		this->stackNetId = other.stackNetId;
		return this;
	};
};
#pragma pack(pop)
static_assert(sizeof(ItemStack) == 0xA0);