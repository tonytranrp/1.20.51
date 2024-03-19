#pragma once

#include "../Features/Modules/EnchantWeightSetting.h"

#include "WindowScreen.h"

#include <memory>
#include <set>

class EnchantWeightScreenData : public ScreenData
{
	std::shared_ptr<EnchantWeightSetting> weightSetting = nullptr;
public:
	EnchantWeightScreenData(std::shared_ptr<EnchantWeightSetting> weightSetting);
	std::shared_ptr<EnchantWeightSetting> getWeightsAccess() const;
};

class EnchantWeightScreen : public WindowScreen
{
	std::shared_ptr<EnchantWeightSetting> weightSetting = nullptr;
	std::set<EnchantType> enchantsSet;
	std::vector<std::string> enchantNames;
	std::string searchQuery;
	int64_t scrollAmount = 0.0f;
	float lastEventScroll = 0.0f;
public:
	EnchantWeightScreen();

	virtual void initialize(ScreenData* data) override;
	virtual void renderWindowContent(float deltaTime) override;
	virtual void handleInput(MouseEvent& event) override;
};