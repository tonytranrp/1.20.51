#pragma once

#include "../Module.h"
#include "../ModeSetting.h"

enum class antiCrystalMode_t
{
	Basic,
	Advanced
};

class PacketEvent;
class AntiCrystal : public Module {
protected:
	std::shared_ptr<ModeSetting<antiCrystalMode_t>> mode = nullptr;
	std::shared_ptr<Setting> downMax = nullptr;
	std::shared_ptr<Setting> hideJump = nullptr;
	std::shared_ptr<Setting> depth = nullptr;
	std::shared_ptr<Setting> forceCentered = nullptr;
	std::shared_ptr<Setting> jumpDisable = nullptr;

	int lastJump = 0;
public:
	AntiCrystal();
	void onEnable() override;
	void onDisable() override;
	void onPacketEvent(PacketEvent& event);
};