#include "SigProvider.h"

#include "../../Mem/Mem.h"

#include <cassert>

SigProvider::SigProvider(const std::string& sourceName) : ManagedItem(sourceName)
{
}

std::string SigProvider::getSource()
{
	return this->getName();
}

void SigProvider::addResult(SigTable sig, void* address)
{
	this->sigResults[sig] = address;
}

void SigProvider::addResult(SigTable sig, const std::string& pattern, int offset)
{
	uintptr_t addr = Mem::FindSig(pattern.c_str());
	assert(addr != 0);

	this->addResult(sig, (void*)(addr + offset));
}

void* SigProvider::findFor(SigTable sig)
{
	if (this->sigResults.contains(sig)) {
		return this->sigResults[sig];
	}
	return nullptr;
}
