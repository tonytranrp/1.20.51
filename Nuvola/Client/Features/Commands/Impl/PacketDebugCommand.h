#pragma once

#include "../Command.h"

class PacketEvent;
class PacketDebugCommand : public Command {
public:
	PacketDebugCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	[[nodiscard]] std::string getDescription() const override;
	[[nodiscard]] std::vector<std::string> getAliases() const override;
	void onPacketEvent(PacketEvent& event);
};