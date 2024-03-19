#pragma once

#include "../Module.h"
#include "../ModeSetting.h"

enum class disablerMode_t {
	Lifeboat,
	Sentinel,
	Duplicate
};

//Mode specific modes
enum class sentinelMode_t
{
	Spam,
	Old
};

class PacketEvent;
class Disabler : public Module {
private:
	std::shared_ptr<ModeSetting<disablerMode_t>> mode;
	std::shared_ptr<ModeSetting<sentinelMode_t>> sentinelMode;
public:
	Disabler();
	void onEnable() override;
	void onDisable() override;
	void onPacketEvent(PacketEvent& event);
};