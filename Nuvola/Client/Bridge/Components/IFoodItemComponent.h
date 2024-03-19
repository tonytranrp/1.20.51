#pragma once

#include "../HashedString.h"

#include <unordered_set>
#include <vector>
#include <string_view>
#include <stdint.h>

class FoodEffect
{
public:
	HashedString name;
	char pad_0030[24]; //0x0030
	int32_t duration; //0x0048
	int32_t amplifier; //0x004C
	float chance; //0x0050
	char pad_0054[4]; //0x0054

	bool isPositive() const
	{
		static const std::unordered_set<std::string_view> positiveEffects = {
			"regeneration", "speed", "haste", "strength", "jump_boost", "night_vision",
			"fire_resistance", "water_breathing", "invisibility", "health_boost", "absorption",
			"saturation", "conduit_power", "dolphin's_grace"
		};
		static const std::unordered_set<std::string_view> negativeEffects = {
			"poison", "nausea", "blindness", "mining_fatigue", "weakness", "slowness",
			"hunger", "levitation", "unluck", "slow_falling", "decay", "instant_damage",
			"glowing", "bad_omen", "hero_of_the_village", "wither"
		};

		return positiveEffects.count(name.get()) > 0;
	}
}; //Size: 0x0058
static_assert(sizeof(FoodEffect) == 0x58);

class IFoodItemComponent
{
public:
	char pad_0000[8]; //0x0000
	class Item* item; //0x0008
	int32_t hungerPoints; //0x0010
	float saturationModifier; //0x0014
	char pad_0018[100]; //0x0018
	bool canAlwaysEat; //0x007C
	char pad_007D[3]; //0x007D
	std::vector<FoodEffect> effects; //0x0080
}; //Size: 0x0098
static_assert(sizeof(IFoodItemComponent) == 0x98);