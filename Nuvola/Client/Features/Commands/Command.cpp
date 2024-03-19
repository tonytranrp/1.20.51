#include "Command.h"

#include "../../Bridge/ClientInstance.h"

#include <xorstr.hpp>

Command::Command(const std::string& name) : ManagedItem(name) {
	
}

GuiData* Command::getGuiData() {
	return Bridge::getClientInstance()->getGuiData();
}

void Command::displayMessage(const std::string& message) {
	getGuiData()->displayClientMessage(message);
}

std::string Command::getDescription() const {
	return "No description provided";
}
std::string Command::getUsage() const {
	return "&cNo usage provided";
}
std::vector<std::string> Command::getAliases() const {
	return {};
}
bool Command::matchName(const std::string_view toMatch) const {
	//If the command name matches, perfect!
	if(toMatch == this->getName()) {
		return true;
	}
	//If not, check for aliases
	const auto aliases = this->getAliases();
	const bool found = std::ranges::any_of(aliases, [=](auto alias)
	{
		//If an alias matches, thats out command
		if (alias == toMatch) {
			return true;
		}
        return false;
	});
	return found;
}