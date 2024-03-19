#pragma once

#include <Data/ManagedItem.h>

#include "IModeSetting.h"
#include "Setting.h"

#include <magic_enum.hpp>

template<typename mode_t>
class ModeSetting :  public IModeSetting
{
public:
	ModeSetting(std::string name, std::string description) :
		IModeSetting(name, description, 0, magic_enum::enum_count<mode_t>() - 1, std::vector<std::string>(magic_enum::enum_names<mode_t>().begin(), magic_enum::enum_names<mode_t>().end()))
	{
	}

	std::shared_ptr<Manager<Setting>> getManagerFor(mode_t mode)
	{
		auto index = magic_enum::enum_index<mode_t>(mode);
		if(index.has_value())
			return this->getItem(*index);
		return nullptr;
	}

	mode_t getCurrentMode()
	{
		return static_cast<mode_t>(this->getValueAs<int>());
	}
};