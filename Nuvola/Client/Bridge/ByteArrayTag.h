#pragma once

#include "Tag.h"
#include "TagMemoryChunk.h"

class ByteArrayTag : public Tag
{
public:
	TagMemoryChunk val;
};