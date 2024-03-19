#pragma once

#include "Item.h"
#include "Macro.h"

enum class ArmorSlot : int32_t
{
	HEAD = 0,
	CHESTPLATE = 1,
	LEGGINGS = 2,
	BOOTS = 3
};

class ArmorItem : public Item
{
public:
	ArmorSlot armorSlot; //0x0258
	int32_t armorValue; //0x025C
	char pad_0260[16]; //0x0260

	int getArmorValue() const
	{
		return this->armorValue;
		//return vftCall<64, float, const Item*>(this);
	}
	float getArmorKnockbackResistance() const
	{
		return vftCall<136, float, const ArmorItem*>(this);
	}
};
static_assert(sizeof(ArmorItem) == 0x270);