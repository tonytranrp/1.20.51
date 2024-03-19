#include "OffsetProvider.h"

OffsetProvider::OffsetProvider(std::string sourceName) : ManagedItem(sourceName) {}

std::string OffsetProvider::getSource()
{
	return this->getName();
}

void OffsetProvider::addResult(OffsetTable offset, int32_t address)
{
	this->offsetResults[offset] = address;
}

int32_t OffsetProvider::findFor(OffsetTable offset)
{
	return this->offsetResults[offset];
}

bool OffsetProvider::has(OffsetTable offset)
{
	return this->offsetResults.contains(offset);
}
