#pragma once

#include "Theme.h"

#include "../../SystemManager.h"

#include <Data/Manager.h>

#include <vector>

class ThemeManager : public ::System<Manager<Theme>>
{
	std::vector<std::shared_ptr<ThemeVariable>> builtins;
	std::shared_ptr<Theme> lastTheme;
	std::shared_ptr<Theme> active;
	float changeProgress = 0.0f;
public:
	ThemeManager();
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;

	template<typename T>
	void addBuiltInVar()
	{
		std::shared_ptr<T> var = std::make_shared<T>();
		this->builtins.push_back(std::move(var));
	}
	bool hasBuiltInVar(std::string name);
	std::shared_ptr<ThemeVariable> buildBuiltIn(std::string name, std::string params);

	void nextFrame();
	Theme getActiveTheme();
	void setActiveTheme(const std::shared_ptr<Theme>& theme);
	bool reloadAllThemes();
	std::shared_ptr<Theme> loadFromDisk(std::string path);
	const std::vector<std::shared_ptr<Theme>>& getThemes();
};