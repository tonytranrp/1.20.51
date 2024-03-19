#include "SwingCommand.h"

#include "../../../Bridge/ClientInstance.h"
#include "../../../Bridge/LocalPlayer.h"
#include "../../../Bridge/GuiData.h"

SwingCommand::SwingCommand() : Command("swing") {

}

void SwingCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("swing")
		.executes([](bcs::command_context context) -> bool {
			ClientInstance* client = Bridge::getClientInstance();
			LocalPlayer* player = client->getClientPlayer();

			player->swing();

			return true;
		})
	);
}
std::string SwingCommand::getDescription() const {
	return "Arbitrarily swings your arm";
}