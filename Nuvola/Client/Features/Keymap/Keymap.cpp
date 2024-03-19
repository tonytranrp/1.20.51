#include "Keymap.h"

#include "../../SystemManager.h"
#include "../Modules/ModuleManager.h"

static std::string invalid = "NULL";

Keymap::Keymap() : ManagedItem(invalid)
{
	this->keymap = nlohmann::json::object();
}

void Keymap::loadJson(std::string jsonStr)
{
	try
	{
		this->loadJson(nlohmann::json::parse(jsonStr));
	}
	catch (std::exception& ex)
	{
		this->keymap = nlohmann::json::object();
	}
}

void Keymap::loadJson(nlohmann::json document)
{
	if (document.is_object())
		this->keymap = document;
	else
		this->keymap = nlohmann::json::object();
}

const nlohmann::json& Keymap::saveJson()
{
	return this->keymap;
}

std::string Keymap::getName() const
{
	if (this->keymap.contains("meta"))
	{
		if (this->keymap["meta"].contains("name"))
		{
			return this->keymap["meta"]["name"].get<std::string>();
		}
	}
	return invalid;
}

void Keymap::setName(std::string name)
{
	if (this->keymap.contains("meta"))
	{
		this->keymap["meta"]["name"] = name;
	}
	else
	{
		this->keymap["meta"] = {
			{ "name", name },
		};
	}
}

void Keymap::captureFromClient()
{
	if (!this->keymap.contains("modules"))
	{
		this->keymap["modules"] = {};
	}

	nlohmann::json& modulesJson = this->keymap["modules"];

	const auto moduleManager = systems->getModuleManager();
	const auto modules = moduleManager->getAllModules();
	for (const auto module : modules)
	{
		if (!modulesJson.contains(module->getName()))
		{
			modulesJson[module->getName()] = {};
		}
		nlohmann::json& moduleJson = modulesJson[module->getName()];
		moduleJson["keybind"] = module->getHotkey();
	}
}

void Keymap::applyToClient()
{
	//No modules in the config to apply
	if (!this->keymap.contains("modules"))
	{
		return;
	}

	const auto moduleManager = systems->getModuleManager();
	const auto modules = moduleManager->getAllModules();
	for (const auto module : modules)
	{
		const std::string& name = module->getName();
		if (!this->keymap["modules"].contains(name))
		{
			//This module is not saved in the config
			continue;
		}

		if (!this->keymap["modules"][name].contains("keybind"))
		{
			//Settings are not saved for this module
			continue;
		}
		module->setHotkey(this->keymap["modules"][name]["keybind"].get<int>(), false);
	}
}
