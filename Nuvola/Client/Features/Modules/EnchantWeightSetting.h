#pragma once

#include "../../Bridge/Enchant.h"

#include "Setting.h"

#include <map>
#include <set>

class EnchantWeightSetting : public Setting
{
	std::map<EnchantType, float> weights;
	std::map<EnchantType, float> defaultWeight;
public:
	EnchantWeightSetting(std::string name, std::string description, std::map<EnchantType, float> defaultWeight = {}) : Setting(name, description, SettingType::ENCHANT_WEIGHTS, nullptr, nullptr, nullptr), weights(defaultWeight), defaultWeight(defaultWeight) {};

	void addWeight(EnchantType enchant, float weight = 0.0f);
	void removeWeight(EnchantType enchant);
	float getWeight(EnchantType enchant);
	std::set<EnchantType> getAddedEnchants();
	void resetToDefault();
};