#include "IPatch.h"

#include "../SystemManager.h"
#include "../Signatures/SigManager.h"

IPatch::IPatch(const std::string& name) : ManagedItem(name)
{
	this->signatures = new std::vector<SigTable>();
}
IPatch::~IPatch() {
	if(!cleaned) {
		autoCleanup();
	}
}
auto IPatch::manualCleanup() -> bool {
	cleaned = false;
	return false;
}
auto IPatch::autoCleanup() -> bool {
	//Clear hook & unhook
    hook->unHook();

	cleaned = true;
	return true;
}
void IPatch::addSignature(SigTable sig)
{
	this->signatures->push_back(sig);
}
uintptr_t IPatch::scanSigs() {
	for (SigTable sig : *this->signatures) {
		auto result = systems->getSigManager()->findAddressAs<uintptr_t>(sig);
		if (result) {
			return result;
		}
	}

	return 0;
}
bool IPatch::apply()
{
    return false;
}
bool IPatch::autoPatch(void* callbackPtr, uintptr_t* funcOriginal) {
	uintptr_t hookAddr = this->scanSigs();

	if(hookAddr == 0) {
		return false;
	}

    hook = std::make_unique<PLH::x64Detour>((uint64_t)hookAddr, (uint64_t)callbackPtr, funcOriginal);
	if(!hook->hook()) {
		return false;
	}

	return true;
}