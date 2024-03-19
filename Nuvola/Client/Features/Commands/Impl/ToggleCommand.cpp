#include "ToggleCommand.h"

#include "../../../SystemManager.h"
#include "../../Modules/ModuleManager.h"

ToggleCommand::ToggleCommand() : Command("toggle") {

}

void ToggleCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("toggle", "t")
		.then(
			bcs::argument<std::string>("module")
			.executes([](bcs::command_context context) -> bool {
				std::string moduleName = context.get<std::string>("module");

				const auto mod = systems->getModuleManager()->findModule(moduleName);
				if (mod == nullptr) {
					displayMessage("&cUnknown module \"" + moduleName + "\"");
					return false;
				}

				mod->toggle();
				if (mod->isEnabled())
					displayMessage("&aEnabled " + mod->getName());
				else
					displayMessage("&cDisabled " + mod->getName());

				return true;
			})
		)
	);
}

std::string ToggleCommand::getDescription() const {
	return "Toggles a module &aon&r&7 or &coff&r&7";
}

std::vector<std::string> ToggleCommand::getAliases() const {
	return { "t" };
}