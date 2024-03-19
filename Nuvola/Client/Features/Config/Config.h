#pragma once

#include <Data/ManagedItem.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

class Config : public ManagedItem
{
	nlohmann::json config;
public:
    Config();

	void loadJson(std::string jsonStr);
	void loadJson(nlohmann::json document);
	const nlohmann::json& saveJson();
	[[nodiscard]] std::string getName() const override;
	void setName(std::string name);
	[[nodiscard]] std::string getAuthor() const;
	void setAuthor(std::string author);
	[[nodiscard]] bool madeByUser() const;
	void captureFromClient();
	void applyToClient() const;
};