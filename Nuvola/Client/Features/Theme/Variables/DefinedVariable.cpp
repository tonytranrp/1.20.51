#include "DefinedVariable.h"

#include "../Theme.h"

DefinedVariable::DefinedVariable(std::string name, ColorF color) : ThemeVariable()
{
	this->name = name;
	this->color = color;
}

std::string DefinedVariable::getName()
{
	return this->name;
}

ColorF DefinedVariable::getColor()
{
	return this->color;
}