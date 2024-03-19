#pragma once

#include "../Module.h"
#include "../EntityTypeListSetting.h"

#include <Math/Math.h>

#include <optional>

class CameraDeltaEvent;
class RenderEvent;
class Aimbot : public Module
{
	std::shared_ptr<Setting> speed = nullptr;
	std::shared_ptr<Setting> range = nullptr;
	std::shared_ptr<Setting> MouseHeld = nullptr;
	std::shared_ptr<EntityTypeListSetting> targets = nullptr;
	std::shared_ptr<Setting> checkTeam = nullptr;

	//State
	std::optional<AABB<float>> targetBox;
public:
	Aimbot();
	virtual void onEnable() override;
	virtual void onDisable() override;

	void onCameraDeltaEvent(CameraDeltaEvent& event);
	void onRenderEvent(RenderEvent& event);
};