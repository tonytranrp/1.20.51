#include "EnchantWeightSetting.h"

#include <algorithm>
#include <iterator>

void EnchantWeightSetting::addWeight(EnchantType enchant, float weight)
{
	this->weights[enchant] = weight;
}

void EnchantWeightSetting::removeWeight(EnchantType enchant)
{
	if (this->weights.contains(enchant))
	{
		this->weights.erase(enchant);
	}
}

float EnchantWeightSetting::getWeight(EnchantType enchant)
{
	if (this->weights.contains(enchant))
	{
		return this->weights.at(enchant);
	}
	return 0.0f;
}

std::set<EnchantType> EnchantWeightSetting::getAddedEnchants()
{
	std::set<EnchantType> enchants;
	std::transform(this->weights.begin(), this->weights.end(),
		std::inserter(enchants, enchants.end()),
		[](auto pair) { return pair.first; }
	);
	return enchants;
}

void EnchantWeightSetting::resetToDefault()
{
	this->weights = this->defaultWeight;
}
