#pragma once

#include "../Module.h"

class PlayerSlowedByItemEvent;
class NoSlowdown : public Module {
public:
	NoSlowdown();
	void onEnable() override;
	void onDisable() override;

	void onPlayerSlowedByItemEvent(PlayerSlowedByItemEvent& event);
};