#pragma once

#include "../Command.h"

class SwingCommand : public Command {
public:
	SwingCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	[[nodiscard]] std::string getDescription() const override;
};