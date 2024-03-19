#pragma once

#include <string>
#include <vector>

class MCKeyBind
{
public:
	std::string bindName;
	std::vector<int> bindKey; //0x0020
	char pad_0038[8]; //0x0038
}; //Size: 0x0040