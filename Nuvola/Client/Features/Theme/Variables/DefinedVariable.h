#pragma once

#include "../Theme.h"

class DefinedVariable : public ThemeVariable
{
	std::string name;
	ColorF color;
public:
	DefinedVariable(std::string name, ColorF color);

	virtual std::string getName() override;
	virtual ColorF getColor() override;
};