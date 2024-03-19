#pragma once

#include "Option.h"

#include <array>

class Options
{
public:
	enum class OptionID : int32_t
	{
		game_thridperson = 3,
		gfx_gamma = 52
	};

	char pad_0000[16]{}; //0x0000
	std::array<Option*, 0x2C0> options;

	//1.20.51.1 Sig to the game's getOption? 48 89 5c 24 ? 48 89 74 24 ? 57 48 81 ec ? ? ? ? 33 c0 81 fa
	//Anyway this does the same thing, whatever
	template<typename option_t>
	option_t* getOption(OptionID id) const
	{
		return reinterpret_cast<option_t*>(this->options[static_cast<int32_t>(id)]);
	}

	template<typename option_t, OptionID id>
	option_t* getOption() const
	{
		return reinterpret_cast<option_t*>(this->options[static_cast<int32_t>(id)]);
	}

	virtual ~Options() = default;
}; //Size: 0x14E0