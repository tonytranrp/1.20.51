#pragma once

#include "../System.h"

#include <Data/ManagedItem.h>
#include <Graphics/imfx.h>

class Preferences : public ::System<ManagedItem>
{
public:
	Preferences();
	void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;

    //If the client should use the keymaps from configs
	bool configKeymap = false;
	//The quality setting for graphics
	ImFX::FXMode graphicsMode = ImFX::FXMode::QUALITY;
	//The username for IRC
	std::string ircName = "Anonymous";

	nlohmann::json serialize();
	void deserialize(nlohmann::json json);
	void load();
	void save();
	void runPayloadAndSave();
};