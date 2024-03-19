#pragma once

#include "../Command.h"

class VersionCommand : public Command {
public:
	VersionCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	[[nodiscard]] std::string getDescription() const override;
	[[nodiscard]] std::vector<std::string> getAliases() const override;
};