#pragma once

#include "../Features/Modules/Category.h"
#include "../Features/Modules/Module.h"
#include "WindowScreen.h"

#include <Data/Manager.h>
#include <Math/Math.h>

#include <optional>
#include <memory>
#include <string>

class ImDrawList;
typedef unsigned int ImGuiID;
class ClickGuiScreen : public WindowScreen
{
	std::shared_ptr<Category> activeCategory = nullptr;
	std::map<ImGuiID, float> animProgs;
	std::map<Module*, bool> changingKey;
	std::map<Module*, bool> moduleExpanded;
	ImGuiID activeMenu = 0;
	int settingsPage = -1;
	std::string searchQuery;
	int nextThemeIndex = -1;

	std::optional<Vector2<float>> windowPos;
public:
	ClickGuiScreen();

	bool drawHotkeyControl(ImDrawList* drawList, std::shared_ptr<Module> module, Vector4<float> controlBox);
	void drawSettingsRecursive(ImDrawList*, const std::shared_ptr<Manager<Setting>>& settings, Vector2<float>& pos, Vector4<float> contentClip, float expandProg);
	void drawModuleContent(ImDrawList* drawList, const std::shared_ptr<Module>& module, Vector2<float> pos, Vector4<float> contentClip, Vector4<float>& moduleRegion);
	bool drawModuleControl(ImDrawList* drawList, const std::shared_ptr<Module>& module, Vector2<float> pos, Vector4<float> contentClip);
	void drawCategoryContent(ImDrawList* drawList, const std::shared_ptr<class Category>& category, Vector4<float> contentClip);
	bool drawCategoryControl(ImDrawList* drawList, const std::shared_ptr<class Category>& category, Vector2<float> pos);

	bool drawTileControl(ImDrawList* drawList, std::string tileTitle, std::string tileSubtitle, Vector4<float> contentClip);
	bool drawPreferencesMenu(ImDrawList* drawList, Vector4<float> contentClip);
	bool drawThemeMenu(ImDrawList* drawList, Vector4<float> contentClip);
	bool drawConfigMenu(ImDrawList* drawList, Vector4<float> contentClip);
	bool drawMenuControl(ImDrawList* drawList, enum class ThemeIcon icon, Vector4<float> controlBox);
	bool drawSettingsPageSelector(ImDrawList* drawList, std::string title, int* pageId, const std::vector<std::string>& pages, Vector4<float> contentClip);
	bool drawSettingsMenu(ImDrawList* drawList, Vector4<float> contentClip);

	virtual void initialize(ScreenData* data) override;
	virtual void renderWindowContent(float deltaTime) override;
	virtual void render(float deltaTime) override;
	virtual void cleanup() override;
	virtual void handleInput(KeyEvent& event) override;
	virtual void handleInput(MouseEvent& event) override;
	virtual void closeBehavior() override;
};