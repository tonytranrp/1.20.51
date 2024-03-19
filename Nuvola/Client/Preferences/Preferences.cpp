#include "Preferences.h"

#include <magic_enum.hpp>
#include <Files/Files.h>

Preferences::Preferences() : System<ManagedItem>("Preferences"),
								configKeymap(false),
								graphicsMode(ImFX::FXMode::QUALITY),
								ircName("Anonymous") {}
void Preferences::install(const std::shared_ptr<SystemInterface> &ownerPtr) {
	System::install(ownerPtr);

	this->load();
}

void Preferences::save()
{
	const std::string preferencesFile = Files::GetUniformDir() + "\\preferences.json";
	const auto file = Files::OpenFile(preferencesFile);

	const nlohmann::json& json = this->serialize();
	const std::string configJson = json.dump(1, '\t');
	Files::SaveFile(file, configJson);
	Files::CloseFile(file);
}
void Preferences::runPayloadAndSave()
{
	ImFX::SetFXMode(this->graphicsMode);
	this->save();
}
void Preferences::load()
{
	const std::string preferencesFile = Files::GetUniformDir() + "\\preferences.json";
	const auto file = Files::OpenFile(preferencesFile);
	if (file == nullptr)
	{
		return;
	}

	const std::pair<char*, size_t> dataB = Files::ReadFile(file);
	Files::CloseFile(file);

	std::string dataStr = std::string(dataB.first, dataB.second);
	try
	{
		this->deserialize(nlohmann::json::parse(dataStr));
	}
	catch (std::exception& ex)
	{
		//Idk
	}
}

nlohmann::json Preferences::serialize()
{
	nlohmann::json serialized;
	serialized["config_keymap"] = this->configKeymap;
	serialized["graphics_mode"] = magic_enum::enum_name(this->graphicsMode);
	serialized["irc_name"] = this->ircName;
	return serialized;
}

void Preferences::deserialize(nlohmann::json json)
{
	if (json.is_object())
	{
		if (json.contains("config_keymap") && json["config_keymap"].is_boolean())
		{
			this->configKeymap = json["config_keymap"].get<bool>();
		}
		if (json.contains("graphics_mode") && json["graphics_mode"].is_string())
		{
			const std::string modeName = json["graphics_mode"].get<std::string>();
			for (auto& value : magic_enum::enum_values<ImFX::FXMode>())
			{
				const auto name = magic_enum::enum_name(value);
				if (name == modeName)
				{
					this->graphicsMode = value;
					break;
				}
			}
		}
		if(json.contains("irc_name") && json["irc_name"].is_string())
		{
			this->ircName = json["irc_name"].get<std::string>();
		}
	}
}
