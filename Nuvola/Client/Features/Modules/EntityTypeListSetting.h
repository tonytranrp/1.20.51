#pragma once

#include "Setting.h"

#include <set>
#include <string>

class EntityTypeListSetting : public Setting
{
	std::set<std::string> selected;
public:
	EntityTypeListSetting(std::string name, std::string description);

	//Returns the set of selected canonical entity types
	const std::set<std::string>& getSelectedTypes();
	void setSelectedTypes(std::set<std::string> selected);
	//Checks if a canonical entity type is selected
	bool isSelected(const std::string& entityType);
	
	void select(const std::string& entityType);
	void deselect(const std::string& entityType);
};