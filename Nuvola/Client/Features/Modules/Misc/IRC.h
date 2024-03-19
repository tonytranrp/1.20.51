#pragma once

#include "../Module.h"

#include <codecvt>
#include <ctime>
#include <mutex>
#include <string>
#include <vector>

class ChatEvent;
class LevelTickEvent;
class IRC : public Module {
	time_t lastConn = 0;

	std::shared_ptr<Setting> showJoinLeave = nullptr;
	std::vector<std::string> messageQueue;
	std::mutex queueLock;
public:
	IRC();
	void onEnable() override;
	void onDisable() override;
	void onTick() override;

	void onChatEvent(ChatEvent& event);
	void onLevelTickEvent(LevelTickEvent& event);
};