#include "EntityTypeListSetting.h"

EntityTypeListSetting::EntityTypeListSetting(std::string name, std::string description) : Setting(name, description, SettingType::ENTITY_TYPES, nullptr, nullptr, nullptr)
{

}

const std::set<std::string>& EntityTypeListSetting::getSelectedTypes()
{
	return this->selected;
}

void EntityTypeListSetting::setSelectedTypes(std::set<std::string> selected)
{
	this->selected = selected;
}

bool EntityTypeListSetting::isSelected(const std::string& entityType)
{
	return this->selected.contains(entityType);
}

void EntityTypeListSetting::select(const std::string& entityType)
{
	this->selected.insert(entityType);
}

void EntityTypeListSetting::deselect(const std::string& entityType)
{
	this->selected.erase(entityType);
}