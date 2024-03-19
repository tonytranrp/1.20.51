#pragma once

#include "Ability.h"

#include <magic_enum.hpp>

#include <array>

enum class AbilitiesIndex {
	Build = 0,
	Mine = 1,
	DoorsAndSwitches = 2,
	OpenContainers = 3,
	AttackPlayers = 4,
	AttackMobs = 5,
	OperatorCommands = 6,
	Teleport = 7,
	Invulnerable = 8,
	Flying = 9,
	MayFly = 10,
	Instabuild = 11,
	Lightning = 12,
	FlySpeed = 13,
	WalkSpeed = 14,
	Muted = 15,
	WorldBuilder = 16,
	NoClip = 17,
	PrivilegedBuilder = 18
};

class Abilities
{
	std::array<Ability, 19> abilities; //0x0004

public:
	[[nodiscard]] const auto& getAbility(AbilitiesIndex index) const
	{
		auto indexVal = magic_enum::enum_index<AbilitiesIndex>(index);

		if (indexVal.has_value() && indexVal < magic_enum::enum_count<AbilitiesIndex>()) {
			return this->abilities.at(*indexVal);
		}
		return INVALID_ABILITY;
	}

	void setFloat(AbilitiesIndex index, float value)
	{
		auto indexVal = magic_enum::enum_index<AbilitiesIndex>(index);

		if (indexVal.has_value() && indexVal < magic_enum::enum_count<AbilitiesIndex>()) {
			this->abilities.at(*indexVal).setFloat(value);
		}
	}

	void setBool(AbilitiesIndex index, bool value)
	{
		auto indexVal = magic_enum::enum_index<AbilitiesIndex>(index);

		if (indexVal.has_value() && indexVal < magic_enum::enum_count<AbilitiesIndex>()) {
			this->abilities.at(*indexVal).setBool(value);
		}
	}

	[[nodiscard]] float getFloat(AbilitiesIndex index) const
	{
		return this->getAbility(index).getFloat();
	}

	[[nodiscard]] bool getBool(AbilitiesIndex index) const
	{
		return this->getAbility(index).getBool();
	}

	static inline Ability INVALID_ABILITY;
};
static_assert(sizeof(Abilities) == 0xE4);