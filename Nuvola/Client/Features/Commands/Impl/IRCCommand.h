#pragma once

#include "../Command.h"

class IRCCommand : public Command {
public:
	IRCCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	std::string getDescription() const override;
	std::string getUsage() const override;
};