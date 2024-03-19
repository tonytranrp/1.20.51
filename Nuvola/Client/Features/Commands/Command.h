#pragma once

#include "Based/bcs.h"

#include <Data/ManagedItem.h>

#include <string>
#include <vector>

class Command : public ManagedItem {
	static class GuiData* getGuiData();
public:
	explicit Command(const std::string& name);
	virtual void initialize(bcs::command_dispatcher& dispatcher) = 0;

	//The command description
	[[nodiscard]] virtual std::string getDescription() const;
	//The command usage
	[[nodiscard]] virtual std::string getUsage() const;
	//Possible aliases (ex. version can shorten to ver)
	[[nodiscard]] virtual std::vector<std::string> getAliases() const;
	//Check the name & alias. If name == command.name OR if name is an alias, return true
	[[nodiscard]] bool matchName(std::string_view toMatch) const;
	//Helper function for output. Parses any formatting
	static void displayMessage(const std::string& message);
};