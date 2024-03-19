#pragma once

#include "../Module.h"

class FogGetColorEvent;
class GetCloudHeightEvent;

class CustomSky : public Module {
	std::shared_ptr<Setting> r;
	std::shared_ptr<Setting> g;
	std::shared_ptr<Setting> b;
	std::shared_ptr<Setting> rainbow;
	std::shared_ptr<Setting> intensity;
	std::shared_ptr<Setting> CloudHeight;
public:
	CustomSky();
	void onEnable() override;
	void onDisable() override;
	void onFogGetColorEvent(FogGetColorEvent& event);
	void onGetCloudHeightEvent(GetCloudHeightEvent& event);
};