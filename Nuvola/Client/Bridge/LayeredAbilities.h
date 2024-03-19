#pragma once

#include "Abilities.h"
#include "PermissionsHandler.h"

#include <cstdint>

enum class AbilitiesLayer
{
	UNKNOWN,
	PLAYER,
	UNKNOWN2,
	UNKNOWN3,
	UNKNOWN4
};

class LayeredAbilities
{
public:
	PermissionsHandler permissions; //0x000
	std::array<Abilities, 5> layers; //0x0004

	[[nodiscard]] const auto& getAbility(AbilitiesLayer layer, AbilitiesIndex index) const
	{
		auto layerValue = magic_enum::enum_index(layer);
		auto indexValue = magic_enum::enum_index(index);

		if (layerValue.has_value() && *layerValue < magic_enum::enum_count<AbilitiesLayer>()) {
			const auto& abilityLayer = this->layers.at(*layerValue);
			if (indexValue.has_value() && *indexValue < magic_enum::enum_count<AbilitiesIndex>()) {
				return abilityLayer.getAbility(index);
			}
		}
		return Abilities::INVALID_ABILITY;
	}

	void setAbility(AbilitiesLayer layer, AbilitiesIndex index, float value)
	{
		auto layerValue = magic_enum::enum_index(layer);
		auto indexValue = magic_enum::enum_index(index);

		if (layerValue.has_value() && *layerValue < magic_enum::enum_count<AbilitiesLayer>()) {
			auto& abilityLayer = this->layers.at(*layerValue);
			if (indexValue.has_value() && *indexValue < magic_enum::enum_count<AbilitiesIndex>()) {
				abilityLayer.setFloat(index, value);
			}
		}
	}

	void setAbility(AbilitiesLayer layer, AbilitiesIndex index, bool value)
	{
		auto layerValue = magic_enum::enum_index(layer);
		auto indexValue = magic_enum::enum_index(index);

		if (layerValue.has_value() && *layerValue < magic_enum::enum_count<AbilitiesLayer>()) {
			auto& abilityLayer = this->layers.at(*layerValue);
			if (indexValue.has_value() && *indexValue < magic_enum::enum_count<AbilitiesIndex>()) {
				abilityLayer.setBool(index, value);
			}
		}
	}
}; //Size: 0x0478
static_assert(sizeof(LayeredAbilities) == 0x478);