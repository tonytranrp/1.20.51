#include "ThemeManager.h"

#include <Files/Files.h>

#include "Themes/NuvolaLight.h"
#include "Variables/Rainbow.h"

#include "imgui.h"

ThemeManager::ThemeManager() : System<Manager<Theme>>("ThemeManager")
{
}

void ThemeManager::install(const std::shared_ptr<SystemInterface>& ownerPtr) {
    System::install(ownerPtr);
    this->addBuiltInVar<Rainbow>();
}

bool ThemeManager::hasBuiltInVar(std::string name)
{
	for (auto& builtin : this->builtins)
	{
		if (builtin->getName() == name)
		{
			return true;
		}
	}
	return false;
}
std::shared_ptr<ThemeVariable> ThemeManager::buildBuiltIn(std::string name, std::string args)
{
	std::stringstream ss(args);
	std::vector<std::string> tokens;

	while (ss.good() && !args.empty()) {
		std::string token;
		std::getline(ss, token, ',');
		tokens.push_back(token);
	}

	for (auto& builtin : this->builtins)
	{
		if (builtin->getName() == name)
		{
			return builtin->buildNew(tokens);
		}
	}
	return nullptr;
}

void ThemeManager::nextFrame()
{
	if (this->changeProgress >= 1.0f)
	{
		this->lastTheme = nullptr;
		this->changeProgress = 0.0f;
	}
	if (this->lastTheme != nullptr)
	{
		this->changeProgress += ImGui::GetIO().DeltaTime * 4.0f;
	}

	if(this->active != nullptr)
		this->active->nextFrame();
}

Theme ThemeManager::getActiveTheme()
{
	if (this->active == nullptr)
	{
		this->addItem<NuvolaLight>();
		this->active = this->getThemes()[0];
	}
	if (this->lastTheme != nullptr)
	{
		return this->lastTheme->interpolate(*this->active, this->changeProgress);
	}
	return *this->active;
}

void ThemeManager::setActiveTheme(const std::shared_ptr<Theme>& theme)
{
	this->lastTheme = this->active;
	this->active = theme;
}

bool ThemeManager::reloadAllThemes()
{
	std::string themesDir = Files::GetUniformDir() + "\\Themes";
	if (!std::filesystem::exists(themesDir))
	{
		std::filesystem::create_directories(themesDir);
		return true;
	}

	this->items.clear();

	this->addItem<NuvolaLight>();

	for (auto file : std::filesystem::directory_iterator(themesDir))
	{
		if (file.is_directory()) { continue; };

		if (this->loadFromDisk(file.path().string()) == nullptr)
		{
			return false;
		}
	}
	return true;
}

std::shared_ptr<Theme> ThemeManager::loadFromDisk(std::string themeFile)
{
	auto file = Files::OpenFile(themeFile);
	if (file == nullptr)
	{
		return nullptr;
	}

	std::pair<char*, size_t> dataB = Files::ReadFile(file);
	std::string dataStr = std::string(dataB.first, dataB.second);
	std::string filename = std::filesystem::path(themeFile).filename().string();

	std::shared_ptr<Theme> theme = this->addItem<Theme>();
	try
	{
		theme->deserialize(nlohmann::json::parse(dataStr, {}, true, true));
	}
	catch (std::exception& ex)
	{
		//A theme failed to load
		theme->addError(ex.what());
	}

	Files::CloseFile(file);

	return theme;
}

const std::vector<std::shared_ptr<Theme>>& ThemeManager::getThemes()
{
	return this->getItems();
}
