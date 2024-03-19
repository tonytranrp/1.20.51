#pragma once

#include "Tag.h"
#include "TagMemoryChunk.h"

class IntArrayTag : public Tag
{
public:
	TagMemoryChunk val;
};