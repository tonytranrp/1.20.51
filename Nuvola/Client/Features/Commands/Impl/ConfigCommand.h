#pragma once

#include "../../Modules/Render/Notifications.h"
#include "../../Modules/ModuleManager.h"

#include "../Command.h"

class ConfigCommand : public Command {
public:
	ConfigCommand();
	void initialize(bcs::command_dispatcher& dispatcher) override;
	std::string getDescription() const override;
	std::vector<std::string> getAliases() const override;
};