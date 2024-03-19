#include "CommandManager.h"

#include "../../Bridge/ClientInstance.h"
#include "../../Bridge/LocalPlayer.h"
#include "../../Events/EventDispatcher.h"
#include "../../SystemManager.h"
#include "../IRC/ChatClient.h"
#include "../Modules/ModuleManager.h"
#include "../Modules/Render/Notifications.h"

#include "Impl/ConfigCommand.h"
#include "Impl/HClipCommand.h"
#include "Impl/HelpCommand.h"
#include "Impl/IRCCommand.h"
#include "Impl/PacketDebugCommand.h"
#include "Impl/SwingCommand.h"
#include "Impl/TeleportCommand.h"
#include "Impl/VClipCommand.h"
#include "Impl/TestCommand.h"
#include "Impl/ToggleCommand.h"
#include "Impl/VersionCommand.h"

#include <stdexcept>

CommandManager::CommandManager() : System<Manager<Command>>("CommandManager") {
	this->depends<ChatClient>();
	this->depends<EventDispatcher>();
}

void CommandManager::install(const std::shared_ptr<SystemInterface> &ownerPtr) {
    System::install(ownerPtr);

    //Register commands
    this->initialize<ConfigCommand>();
    this->initialize<HClipCommand>();
    this->initialize<HelpCommand>();
	this->initialize<IRCCommand>();
    this->initialize<PacketDebugCommand>();
    this->initialize<SwingCommand>();
    this->initialize<VClipCommand>();
    this->initialize<TeleportCommand>();
    this->initialize<TestCommand>();
    this->initialize<ToggleCommand>();
    this->initialize<VersionCommand>();

	auto dispatcher = systems->getEventDispatcher();
	dispatcher->listen<ChatEvent, &CommandManager::onChatEvent>(this);
};

static bool prefixWarned = false;
//Handle sent chat messages here
void CommandManager::onChatEvent(ChatEvent& event) {
	const std::string_view message = event.getMessage();

	const ClientInstance* ci = Bridge::getClientInstance();
	GuiData* gd = ci->getGuiData();
	LocalPlayer* player = ci->getClientPlayer();

	if (message[0] == '.' && !prefixWarned)
	{
		const auto notifs = systems->getModuleManager()->findModule<Notifications>();
		notifs->notify("Commands in Nuvola are prefixed with '!' instead of '.' in an effort to be quirky and unique. This warning will not show again.", "Commands", 15.0f);
		prefixWarned = true;

		event.setCancelled(true);
	}

	if(message[0] == '!') {
		//This is a command

		const std::string_view noPrefix = message.substr(1);
		if(!noPrefix.empty()) {
			try
			{
				this->dispatcher.execute(noPrefix, player);
			}
			catch (const bcs::bcs_error& error)
			{
				gd->displayClientMessage(std::format("&c{}", error.what()));
			}
		}

		event.setCancelled(true);
	}
}