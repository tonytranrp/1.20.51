#pragma once

#include "../../Bridge/Block.h"
#include "Setting.h"

#include <set>
#include <string>

class BlockTypeListSetting : public Setting
{
	std::set<std::string> selected;
public:
	BlockTypeListSetting(std::string name, std::string description);

	//Returns the set of selected blocks
	const std::set<std::string>& getSelectedTypes() const;
	void setSelectedTypes(const std::set<std::string>& selected);
	//Checks if a block is selected
	bool isSelected(const std::string& blockType) const;

	void select(const std::string& blockType);
	void deselect(const std::string& blockType);

	[[nodiscard]] std::set<const Block*> getSelectedBlocks() const;
};