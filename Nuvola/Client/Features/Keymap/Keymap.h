#pragma once

#include <Data/ManagedItem.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

class Keymap final : public ManagedItem
{
	nlohmann::json keymap;
public:
	Keymap();
	void loadJson(std::string jsonStr);
	void loadJson(nlohmann::json document);
	const nlohmann::json& saveJson();
	std::string getName() const override;
	void setName(std::string);
	void captureFromClient();
	void applyToClient();
};