#include "ConfigCommand.h"

#include "../../../SystemManager.h"
#include "../../Config/ConfigManager.h"

#include <Files/Files.h>

#define TOLOWER(string) std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) { return std::tolower(c); })

ConfigCommand::ConfigCommand() : Command("config")
{

}

void ConfigCommand::initialize(bcs::command_dispatcher& dispatcher)
{
	dispatcher.create(
		bcs::literal("config", "c")
		.then(
			bcs::literal("load", "l")
			.then(
				bcs::argument<std::string>("name")
				.executes([](bcs::command_context context) -> bool {
					auto cfgName = context.get<std::string>("name");
					TOLOWER(cfgName);

					auto configMgr = systems->getConfigManager();
					for (const auto& cfg : configMgr->getConfigs())
					{
						std::string name = cfg->getName();
						TOLOWER(name);

						if (cfgName == name)
						{
							cfg->applyToClient();
							displayMessage("&aConfig loaded!");
							return true;
						}
					}

					displayMessage("&cInvalid config!");
					return true;
				})
			)
		)
		.then(
			bcs::literal("save", "s")
			.then(
				bcs::argument<std::string>("name")
				.executes([](bcs::command_context context) -> bool {
					auto confName = context.get<std::string>("name");
					TOLOWER(confName);

					auto configMgr = systems->getConfigManager();
					for (auto cfg : configMgr->getConfigs())
					{
						std::string name = cfg->getName();
						TOLOWER(name);

						if (name == confName)
						{
							cfg->captureFromClient();
							configMgr->saveToDisk(cfg);

							displayMessage("&aConfig saved!");
							return true;
						}
					}

					auto cfg = configMgr->createNew(confName);

					cfg->captureFromClient();
					configMgr->saveToDisk(cfg);

					displayMessage("&aConfig saved!");
					return true;
				})
			)
		)
		.then(
			bcs::literal("delete", "d")
			.then(
				bcs::argument<std::string>("name")
				.executes([](bcs::command_context context) -> bool {
					auto config = context.get<std::string>("name");
					TOLOWER(config);

					auto configMgr = systems->getConfigManager();
					auto configs = configMgr->getItems();

					for (auto& ourConfig : configs)
					{
						std::string configName = ourConfig->getName();
						TOLOWER(configName);

						if (configName == config)
						{
							configMgr->deleteFromDisk(ourConfig);
							configMgr->deleteConfig(ourConfig);
							displayMessage("&aDeleted config!");
							return true;
						}
					}

					displayMessage("&cFailed to delete config.");
					return true;
				})
			)
		)
	);
}

std::string ConfigCommand::getDescription() const {
	return "Load, save, rename, or delete configs with a command";
}

std::vector<std::string> ConfigCommand::getAliases() const {
	std::vector<std::string> aliases;
	aliases.emplace_back("c");
	return aliases;
}