#pragma once

#include "../Command.h"

class TestCommand : public Command {
public:
	TestCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	std::string getDescription() const override;
};