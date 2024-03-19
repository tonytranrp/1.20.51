#pragma once

#include "../Command.h"

class HelpCommand : public Command {
public:
	HelpCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;

	[[nodiscard]] std::string getDescription() const override;
	std::vector<std::string> getAliases() const override;
};