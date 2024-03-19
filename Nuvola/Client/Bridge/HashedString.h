#pragma once

#include <string>

class HashedString {
	size_t hash;
	std::string data;
	HashedString* lastCmp;

public:
	HashedString() = default;
	[[nodiscard]] const std::string& get() const
	{
		return this->data;
	}
	operator const std::string&() const
	{
		return this->get();
	}
};