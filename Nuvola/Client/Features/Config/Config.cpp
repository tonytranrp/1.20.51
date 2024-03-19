#include "Config.h"

#include "../../Preferences/Preferences.h"

#include "../Modules/ModuleManager.h"
#include "../Modules/BlockTypeListSetting.h"
#include "../Modules/EnchantWeightSetting.h"
#include "../Modules/EntityTypeListSetting.h"
#include "../Modules/GroupSetting.h"
#include "../Modules/IModeSetting.h"

#include <magic_enum.hpp>
#include <magic_enum_utility.hpp>

static std::string invalid = "NULL";

void Config::loadJson(std::string jsonStr)
{
	this->loadJson(nlohmann::json::parse(jsonStr));
}

void Config::loadJson(nlohmann::json document)
{
	this->config = document;
}

const nlohmann::json& Config::saveJson()
{
	return this->config;
}

std::string Config::getName() const
{
	if (this->config.contains("meta"))
	{
		if (this->config["meta"].contains("name"))
		{
			return this->config["meta"]["name"].get<std::string>();
		}
	}
	return invalid;
}

void Config::setName(std::string name)
{
	if (this->config.contains("meta"))
	{
		this->config["meta"]["name"] = name;
	}
	else
	{
		this->config["meta"] = {
			{ "name", name },
		};
	}
}

std::string Config::getAuthor() const
{
	if (this->config.contains("meta"))
	{
		if (this->config["meta"].contains("author"))
		{
			return this->config["meta"]["author"].get<std::string>();
		}
	}
	return invalid;
}

void Config::setAuthor(std::string author)
{
	if (this->config.contains("meta"))
	{
		this->config["meta"]["author"] = author;
	}
	else
	{
		this->config["meta"] = {
			{ "author", author },
		};
	}
}

bool Config::madeByUser() const
{
	return true; //this->getAuthor() == currentUser->GetUsername();
}

void captureSettingsRecursive(nlohmann::json& json, const std::vector<std::shared_ptr<Setting>>& settings)
{
	for (auto& setting : settings)
	{
		nlohmann::json& settingObj = json[setting->getName()];
		switch (setting->getType())
		{
		case SettingType::TOGGLE:
			settingObj = setting->getValueAs<bool>();
			break;
		case SettingType::ENUM:
		case SettingType::SLIDER_INT:
		{
			settingObj = setting->getValueAs<int>();

			//If it's a mode setting, we have to capture its settings too
			std::shared_ptr<IModeSetting> modeSetting = std::dynamic_pointer_cast<IModeSetting>(setting);
			if (modeSetting != nullptr)
			{
				settingObj = nlohmann::json::object();
				settingObj["value"] = setting->getValueAs<int>();
				settingObj["modes"] = nlohmann::json::array();
				nlohmann::json& modesJson = settingObj["modes"];
				for (const auto& mode : modeSetting->getItems())
				{
					nlohmann::json settingsObj = nlohmann::json::object();
					captureSettingsRecursive(settingsObj, mode->getItems());
					modesJson.push_back(settingsObj);
				}
			}

			break;
		}
		case SettingType::SLIDER_FLOAT:
			settingObj = setting->getValueAs<float>();
			break;
		case SettingType::ENTITY_TYPES:
		{
			std::shared_ptr<EntityTypeListSetting> casted = std::dynamic_pointer_cast<EntityTypeListSetting>(setting);
			assert(casted != nullptr);
			settingObj = casted->getSelectedTypes();
			break;
		}
		case SettingType::BLOCK_TYPES:
		{
			std::shared_ptr<BlockTypeListSetting> casted = std::dynamic_pointer_cast<BlockTypeListSetting>(setting);
			assert(casted != nullptr);
			settingObj = casted->getSelectedTypes();
			break;
		}
		case SettingType::GROUP:
		{
			std::shared_ptr<GroupSetting> casted = std::dynamic_pointer_cast<GroupSetting>(setting);
			assert(casted != nullptr);
			settingObj["children"] = nlohmann::json::object();
			captureSettingsRecursive(settingObj["children"], casted->getItems());
			break;
		}
		case SettingType::ENCHANT_WEIGHTS:
		{
			std::shared_ptr<EnchantWeightSetting> casted = std::dynamic_pointer_cast<EnchantWeightSetting>(setting);
			assert(casted != nullptr);
			settingObj["weights"] = nlohmann::json::object();
			nlohmann::json& weightsObj = settingObj["weights"];
			magic_enum::enum_for_each<EnchantType>([&](auto val) {
				constexpr EnchantType enchant = val;
				weightsObj[magic_enum::enum_name(enchant)] = casted->getWeight(enchant);
			});
			break;
		}
		case SettingType::TEXT:
		default:
			break;
		};
	}
}

void Config::captureFromClient()
{
	std::string author = this->getAuthor();
	if (author == invalid)
	{
		author = "NO_USERNAME";//currentUser->GetUsername();
	}
	this->setAuthor(author);

	if (!this->config.contains("modules"))
	{
		this->config["modules"] = {};
	}

	nlohmann::json& modulesJson = this->config["modules"];

	const auto moduleManager = systems->getModuleManager();
	const auto modules = moduleManager->getAllModules();
	for (const auto& module : modules)
	{
		if (!modulesJson.contains(module->getName()))
		{
			modulesJson[module->getName()] = {};
		}
		nlohmann::json& moduleJson = modulesJson[module->getName()];
		moduleJson["enabled"] = module->isEnabled();

		moduleJson["hotkey"] = module->getHotkey();

		if (!moduleJson.contains("settings"))
		{
			moduleJson["settings"] = {};
		}
		nlohmann::json& settingsJson = moduleJson["settings"];
		captureSettingsRecursive(settingsJson, module->getItems());
	}
}

void applySettingsRecursive(const std::vector<std::shared_ptr<Setting>>& settings, const nlohmann::json& json)
{
	for (auto& setting : settings)
	{
		//Check the config has the setting
		if (json.contains(setting->getName()))
		{
			const nlohmann::json& settingJson = json[setting->getName()];
			switch (setting->getType())
			{
			case SettingType::TOGGLE:
				setting->setValue(settingJson.get<bool>());
				break;
			case SettingType::ENUM:
			case SettingType::SLIDER_INT:
				if (settingJson.is_number_integer())
				{
					setting->setValue(settingJson.get<int>());
				}
				if (settingJson.is_object())
				{
					std::shared_ptr<IModeSetting> modeSetting = std::dynamic_pointer_cast<IModeSetting>(setting);
					if (modeSetting != nullptr)
					{
						if (settingJson.contains("modes"))
						{
							const nlohmann::json& modesArr = settingJson["modes"];
							if (modesArr.is_array())
							{
								for (int i = 0; i < modesArr.size(); i++)
								{
									const auto subSettings = modeSetting->getManagerFor(i);
									const auto& mode = modesArr[i];
									applySettingsRecursive(subSettings->getItems(), mode);
								}
							}
						}
						if (settingJson.contains("value"))
						{
							const nlohmann::json& value = settingJson["value"];
							if (value.is_number_integer())
							{
								modeSetting->setValue(value.get<int>());
							}
						}
					}
				}
				break;
			case SettingType::SLIDER_FLOAT:
				setting->setValue(settingJson.get<float>());
				break;
			case SettingType::ENTITY_TYPES:
			{
				if(settingJson.is_array())
				{
					std::shared_ptr<EntityTypeListSetting> casted = std::dynamic_pointer_cast<EntityTypeListSetting>(setting);
					assert(casted != nullptr);
					casted->setSelectedTypes(settingJson.get<std::set<std::string>>());
					break;
				}
			}
			case SettingType::BLOCK_TYPES:
			{
				if(settingJson.is_array())
				{
					std::shared_ptr<BlockTypeListSetting> casted = std::dynamic_pointer_cast<BlockTypeListSetting>(setting);
					assert(casted != nullptr);
					casted->setSelectedTypes(settingJson.get<std::set<std::string>>());
					break;
				}
			}
			case SettingType::GROUP:
			{
				if (settingJson.contains("children"))
				{
					const nlohmann::json& children = settingJson["children"];

					std::shared_ptr<GroupSetting> casted = std::dynamic_pointer_cast<GroupSetting>(setting);
					assert(casted != nullptr);
					applySettingsRecursive(casted->getItems(), children);
				}
				break;
			}
			case SettingType::ENCHANT_WEIGHTS:
			{
				if (settingJson.contains("weights"))
				{
					const nlohmann::json& weightsObj = settingJson["weights"];
					if (weightsObj.is_object())
					{
						std::shared_ptr<EnchantWeightSetting> casted = std::dynamic_pointer_cast<EnchantWeightSetting>(setting);
						assert(casted != nullptr);

						magic_enum::enum_for_each<EnchantType>([&](auto val) {
							const EnchantType enchant = val;
							std::string enchantName(magic_enum::enum_name(enchant));

							if (weightsObj.contains(enchantName))
							{
								const nlohmann::json& valueJson = weightsObj[enchantName];
								if (valueJson.is_number_float())
								{
									float valF = valueJson.get<float>();
									casted->removeWeight(enchant);
									casted->addWeight(enchant, valF);
								}
							}
						});
					}
				}
				break;
			}
			case SettingType::TEXT:
			default:
				break;
			};
		}
	}
}

void Config::applyToClient() const
{
	//No modules in the config to apply
	if (!this->config.contains("modules"))
	{
		return;
	}

    auto prefs = systems->getPreferences();
	const auto moduleManager = systems->getModuleManager();
	auto modules = moduleManager->getAllModules();
	for (auto module : modules)
	{
		const std::string& name = module->getName();
		if (!this->config["modules"].contains(name))
		{
			//This module is not saved in the config
			continue;
		}
		if(this->config["modules"][name].contains("enabled"))
			module->setEnabled(this->config["modules"][name]["enabled"].get<bool>());

		if (this->config["modules"][name].contains("hotkey"))
		{
			if (prefs->configKeymap)
			{
				const nlohmann::json& hotkeyJson = this->config["modules"][name]["hotkey"];
				if (hotkeyJson.is_number_integer())
				{
					int hotkeyVal = hotkeyJson.get<int>();
					module->setHotkey(hotkeyVal);
				}
			}
		}

		if (!this->config["modules"][name].contains("settings"))
		{
			//Settings are not saved for this module
			continue;
		}
		const nlohmann::json& settingsObj = this->config["modules"][name]["settings"];
		applySettingsRecursive(module->getItems(), settingsObj);
	}
}

Config::Config() : ManagedItem("ConfigItem")
{

}
