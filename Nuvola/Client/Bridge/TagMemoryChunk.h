#pragma once

#include <memory>

struct TagMemoryChunk
{
	size_t capacity = 0;
	size_t size = 0;
	std::unique_ptr<char[]> data;
};