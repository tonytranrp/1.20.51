#pragma once

#include "../Bridge/NamespacedKey.h"

#include "../Features/Modules/EntityTypeListSetting.h"

#include "TypeSelectionScreen.h"

#include <string>

enum class EntityTypeGroup
{
	Animal,
	Sea,
	Monster,
	Misc,
	Education,
	Custom
};

struct EntityTypeSelectionScreenData : public TypeSelectionScreenData<EntityTypeGroup, std::string>
{
	std::shared_ptr<EntityTypeListSetting> settingPtr = nullptr;
public:
	EntityTypeSelectionScreenData(std::shared_ptr<Setting> settingPtr);

	EntityTypeGroup getGroupFor(NamespacedKey id);
};

class EntityTypeSelectionScreen : public TypeSelectionScreen<EntityTypeGroup, std::string>
{
	std::shared_ptr<EntityTypeListSetting> settingPtr = nullptr;
public:
	EntityTypeSelectionScreen();

	virtual void initialize(ScreenData* data) override;
	virtual void render(float deltaTime) override;
};