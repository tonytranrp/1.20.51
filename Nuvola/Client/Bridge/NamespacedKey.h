#pragma once

#include <string>

struct NamespacedKey {
	/* Fields */
	std::string name;
	std::string key;
	/* Virtuals */
	/* Functions */
	std::string asString()
	{
		return name + ":" + key;
	}
};