#include "VersionCommand.h"

#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/GuiData.h"

#include <Utils/Utils.h>

VersionCommand::VersionCommand() : Command("version") {

}

void VersionCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("version", "ver")
		.executes([](bcs::command_context context) -> bool {
			displayMessage("&7Currently running &aNuvola &b" STRING(NUVOLA_BUILD_TAG) " &a" STRING(NUVOLA_BUILD_VERSION));
			return true;
		})
	);
}

std::string VersionCommand::getDescription() const {
	return "Shows the current Nuvola build version and any other versioning information";
}

std::vector<std::string> VersionCommand::getAliases() const {
	return {"ver"};
}