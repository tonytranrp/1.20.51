#pragma once

#include "../../System.h"

#include "Config.h"

#include <Data/Manager.h>

#include <string>
#include <vector>

class ConfigManager : public ::System<Manager<Config>>
{
public:
    ConfigManager();
    void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;

	const std::vector<std::shared_ptr<Config>>& getConfigs();
	std::shared_ptr<Config> createNew(std::string name);
	void deleteConfig(const std::shared_ptr<Config>& config);
	static bool saveToDisk(const std::shared_ptr<Config>& config);
	bool saveAll();
	std::shared_ptr<Config> loadFromDisk(const std::string& configFile);
	bool reloadAllFromDisk();
	static bool deleteFromDisk(const std::shared_ptr<Config>& config);
};