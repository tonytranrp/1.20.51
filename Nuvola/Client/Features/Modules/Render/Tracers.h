#pragma once

#include "../EntityTypeListSetting.h"
#include "../ModeSetting.h"
#include "../Module.h"

#include <memory>

enum class tracerMode_t : int32_t
{
	Top,
	Center,
	Bottom
};
class RenderEvent;
class Tracers : public Module {
	std::shared_ptr<ModeSetting<tracerMode_t>> sourcePos = nullptr;
	std::shared_ptr<EntityTypeListSetting> targets = nullptr;
	std::shared_ptr<Setting> checkTeam = nullptr;
public:
	Tracers();

	void onEnable() override;
	void onDisable() override;

	void onRenderEvent(RenderEvent& event);
};
