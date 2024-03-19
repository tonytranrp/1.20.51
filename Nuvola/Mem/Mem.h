#pragma once

#include <vector>
#include <string>

#define PI 3.14159265359

class Mem {
public:
    static uintptr_t FindMLvlPtr(uintptr_t, std::vector<unsigned int>);
    static uintptr_t FindSig(std::string_view pattern);
	static uintptr_t FindSig(uintptr_t rangeStart, uintptr_t rangeEnd, std::string_view pattern);
};
