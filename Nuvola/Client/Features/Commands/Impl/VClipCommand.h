#pragma once

#include "../Command.h"

class VClipCommand : public Command {
public:
	VClipCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	[[nodiscard]] std::vector<std::string> getAliases() const override;
	[[nodiscard]] std::string getDescription() const override;
	[[nodiscard]] std::string getUsage() const override;
};