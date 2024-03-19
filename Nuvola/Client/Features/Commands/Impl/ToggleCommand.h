#pragma once

#include "../Command.h"

class ToggleCommand final : public Command {
public:
	ToggleCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	[[nodiscard]] std::string getDescription() const override;
	[[nodiscard]] std::vector<std::string> getAliases() const override;
};