#include "ClientInstanceSigProvider.h"

#include "../../../Mem/Mem.h"

ClientInstanceSigProvider::ClientInstanceSigProvider() : SigProvider("ClientInstance")
{
	//Code in the binary that can be used to calculate the vtable address
	uintptr_t vftAccessor = Mem::FindSig("48 8d 05 ? ? ? ? 48 89 06 48 8d 05 ? ? ? ? 48 89 46 ? 48 8d 05 ? ? ? ? 48 89 86 ? ? ? ? 48 8d 05");
	uintptr_t vftOffsetRef = vftAccessor + 3;
	int32_t vftOffset = *reinterpret_cast<int32_t*>(vftOffsetRef);
	uintptr_t vtablePtr = vftOffsetRef + vftOffset + sizeof(vftOffset);
	this->addResult(SigTable::ClientInstance_vftable, reinterpret_cast<void*>(vtablePtr));

	this->addResult(SigTable::ClientInstance_frameUpdate, std::string("48 89 5c 24 ? 48 89 74 24 ? 57 48 83 ec ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 44 24 ? 48 8b da 48 8b f9 0f 57 c0 0f 11 44 24 ? 0f 11 44 24"));
	this->addResult(SigTable::ClientInstance_update, std::string("48 89 5c 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 44 0f b6 f2 48 8b f1"));
	this->addResult(SigTable::ClientInstance_getInput, std::string("48 89 5c 24 ?? 57 48 83 ec ?? 48 8b f9 48 8b 01 48 8d 54 24"));
}
//40 55 53 56 57 41 54 41 55 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4d 8b f9 4c 89 4d ? 4d 8b f0 48 89 54 24
//48 89 5c 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8d ac 24 ? ? ? ? 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 44 0f b6 f2 48 8b f1
