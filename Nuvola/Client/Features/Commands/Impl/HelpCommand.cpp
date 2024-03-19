#include "HelpCommand.h"

#include "../../../Bridge/LocalPlayer.h"
#include "../../../SystemManager.h"
#include "../CommandManager.h"

HelpCommand::HelpCommand() : Command("help") {

}

void HelpCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal(std::string("help"))
		.then(bcs::literal(std::string("usage")).executes([](bcs::command_context context) -> bool {
			const std::vector<std::shared_ptr<Command>>& commands = systems->getCommandManager()->getAllCommands();
			displayMessage("&ko&r&bNuvola&r&ko&r &8- &eUsage menu");
			for (auto& command : commands) {
				displayMessage("&e> &7!&b" + command->getName() + " &8- &7" + command->getUsage());
			}
			return true;
		}))
		.executes([](bcs::command_context context) -> bool {
			const std::vector<std::shared_ptr<Command>>& commands = systems->getCommandManager()->getAllCommands();

			displayMessage("&ko&r&bNuvola&r&ko&r &8- &eHelp menu");
			for (auto& command : commands) {
				displayMessage("&e> &7!&b" + command->getName() + " &8- &7" + command->getDescription());
			}

			return true;
		})
	);
}

std::string HelpCommand::getDescription() const {
	return "Display's all available Nuvola commands, as well as provides a description of what they each do";
}

std::vector<std::string> HelpCommand::getAliases() const {
	return {"h"};
}