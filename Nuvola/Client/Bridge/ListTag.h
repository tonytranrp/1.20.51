#pragma once

#include "Tag.h"

#include <vector>

class ListTag : public Tag
{
public:
	std::vector<Tag*> val;
	Tag::Type elementType;
};