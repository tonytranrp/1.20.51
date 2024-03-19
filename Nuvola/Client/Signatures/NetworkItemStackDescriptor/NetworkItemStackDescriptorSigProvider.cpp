#include "NetworkItemStackDescriptorSigProvider.h"

NetworkItemStackDescriptorSigProvider::NetworkItemStackDescriptorSigProvider() : SigProvider("NetworkItemStackDescriptor")
{
	this->addResult(SigTable::NetworkItemStackDescriptor_cctor, "48 89 5c 24 ?? 55 56 57 41 56 41 57 48 83 ec ?? 48 8b 05 ?? ?? ?? ?? 48 33 c4 48 89 44 24 ?? 48 8b fa 48 8b f1 48 89 4c 24 ?? 0f b6 5a");
}