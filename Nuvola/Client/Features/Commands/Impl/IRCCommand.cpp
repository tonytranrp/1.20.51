#include "IRCCommand.h"

#include "../../../Preferences/Preferences.h"
#include "../../../SystemManager.h"
#include "../../IRC/ChatClient.h"

IRCCommand::IRCCommand() : Command("irc") {

}

void IRCCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	auto usageMenu = [](bcs::command_context context) -> bool {
		displayMessage("&dIRC&r &8- &eHelp menu");
		displayMessage("&e> &7!&birc &7channel &r(channel: string | undefined) &8- &7Switch to another IRC channel");
		displayMessage("&e> &7!&birc &7name &r(name: string | undefined) &8- &7Change your username displayed in the IRC chat");
		displayMessage("&e> &7!&birc &7users &r(list | count) &8- &7Get a list of users in the channel, or a count of how many are in the channel");
		return true;
	};

	dispatcher.create(
		bcs::literal("irc")
		.then(
			bcs::literal("channel")
			.then(
				bcs::argument<std::string>("channel")
				.executes([](bcs::command_context context) -> bool {
					auto channel = context.get<std::string>("channel");

					auto chatClient = systems->getChatClient();
					ChatClient::ChatOperation operation(ChatClient::OpCodes::SET_CHANNEL, channel, true);
					chatClient->sendOperationAuto(operation);
					return true;
				})
			)
			.executes([](bcs::command_context context) -> bool {
				auto chatClient = systems->getChatClient();
				ChatClient::ChatOperation operation(ChatClient::OpCodes::SET_CHANNEL, "global", true);
				chatClient->sendOperationAuto(operation);
				return true;
			})
		)
		.then(
			bcs::literal("name")
			.then(
				bcs::argument<std::string>("name")
				.executes([](bcs::command_context context) -> bool {
					auto name = context.get<std::string>("name");

					auto prefs = systems->getPreferences();
					prefs->ircName = name;
					prefs->save();

					auto chatClient = systems->getChatClient();
					ChatClient::ChatOperation operation(ChatClient::OpCodes::SET_NAME, name, true);
					chatClient->sendOperationAuto(operation);
					return true;
				})
			)
			.executes([](bcs::command_context context) -> bool {
				auto prefs = systems->getPreferences();
				prefs->ircName = "Anonymous";
				prefs->save();

				auto chatClient = systems->getChatClient();
				ChatClient::ChatOperation operation(ChatClient::OpCodes::SET_NAME, "Anonymous", true);
				chatClient->sendOperationAuto(operation);
				return true;
			})
		)
		.then(
			bcs::literal("users")
			.then(
				bcs::literal("list")
				.executes([](bcs::command_context context) -> bool {
					auto chatClient = systems->getChatClient();
					ChatClient::ChatOperation operation(ChatClient::OpCodes::REQUEST_CONNECTED_NAMES, "", true);
					chatClient->sendOperationAuto(operation);
					return true;
				})
			)
			.then(
				bcs::literal("count")
				.executes([](bcs::command_context context) -> bool {
					auto chatClient = systems->getChatClient();
					ChatClient::ChatOperation operation(ChatClient::OpCodes::REQUEST_CONNECTED_COUNT, "", true);
					chatClient->sendOperationAuto(operation);
					return true;
				})
			)
			.executes(usageMenu)
		)
		.executes(usageMenu)
	);
}

std::string IRCCommand::getDescription() const {
	return "Configure your Nuvola IRC settings";
}

std::string IRCCommand::getUsage() const {
	return "Correct Usage: !irc (subcommand: string)";
}