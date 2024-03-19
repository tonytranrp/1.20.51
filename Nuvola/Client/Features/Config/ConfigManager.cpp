#include "ConfigManager.h"

#include <Files/Files.h>

#include <filesystem>

ConfigManager::ConfigManager() : System<Manager<Config>>("ConfigManager")
{

}

const std::vector<std::shared_ptr<Config>>& ConfigManager::getConfigs()
{
	return this->getItems();
}

std::shared_ptr<Config> ConfigManager::createNew(std::string name)
{
    auto config = this->addItem<Config>();
	config->setName(name);
	return config;
}

void ConfigManager::deleteConfig(const std::shared_ptr<Config>& config)
{
	this->removeItem(config);
}

bool ConfigManager::saveToDisk(const std::shared_ptr<Config>& config)
{
	std::string configDir = Files::GetUniformDir() + "\\Configs";
	std::string configFile = configDir + "\\" + config->getName() + ".json";
	auto file = Files::OpenFile(configFile);

	if (file == nullptr)
	{
		return false;
	}

	std::string configJson = config->saveJson().dump(1, '\t');
	Files::SaveFile(file, configJson);
	Files::CloseFile(file);
	return true;
}

bool ConfigManager::saveAll()
{
    return std::ranges::all_of(this->items, [&](auto config) {
        if (!this->saveToDisk(config))
        {
            return false;
        }
        return true;
    });
}

std::shared_ptr<Config> ConfigManager::loadFromDisk(const std::string& configFile)
{
	auto file = Files::OpenFile(configFile);
	if (file == nullptr)
	{
		return nullptr;
	}

	std::pair<char*, size_t> dataB = Files::ReadFile(file);
	std::string dataStr = std::string(dataB.first, dataB.second);
	std::string filename = std::filesystem::path(configFile).filename().string();

	auto config = this->createNew(filename);
	config->loadJson(nlohmann::json::parse(dataStr));

	Files::CloseFile(file);

	return config;
}

bool ConfigManager::reloadAllFromDisk()
{
	this->items.clear();

	std::string configDir = Files::GetUniformDir() + "\\Configs";
	if (!std::filesystem::exists(configDir))
	{
		std::filesystem::create_directories(configDir);
		return true;
	}

    auto dirEntries = std::filesystem::directory_iterator(configDir);
    return std::ranges::all_of(dirEntries, [&](auto entry) {
        if (entry.is_directory()) { return true; };

        if (this->loadFromDisk(entry.path().string()) == nullptr)
        {
            return false;
        }
        return true;
    });
}

bool ConfigManager::deleteFromDisk(const std::shared_ptr<Config>& config)
{
	std::filesystem::path configDir = Files::GetUniformDir() + "\\Configs";
	if (!std::filesystem::exists(configDir))
	{
		std::filesystem::create_directories(configDir);
		return true;
	}
	std::filesystem::path configFile = configDir / (config->getName() + ".json");

	for (auto& file : std::filesystem::directory_iterator(configDir))
	{
		if (file.is_directory()) { continue; };

		if (file.path().filename() == configFile.filename())
		{
			return Files::DeleteFile(file.path().string());
		}
	}
	return true;
}

void ConfigManager::install(const std::shared_ptr<SystemInterface> &ownerPtr) {
    System::install(ownerPtr);
}
