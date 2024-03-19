#include "GlobalSigProvider.h"

#include "../../../Mem/Mem.h"

#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

GlobalSigProvider::GlobalSigProvider() : SigProvider("Global")
{
	this->addResult(SigTable::Global_cameraDelta, std::string("48 89 5c 24 ?? 57 48 83 ec ?? f3 41 0f 10 00 49 8b d8 f3 41 0f 58 40"));
	this->addResult(SigTable::Global_mouseAction, std::string("48 8B ?? ?? 89 ?? ?? ?? 89 ?? ?? ?? 89 ?? ?? 57 41 ?? 41 ?? 41 ?? 41 ?? 48 83 ?? ?? 44 ?? ?? ?? ?? ?? ?? ?? ?? 48"));
	this->addResult(SigTable::Global_keyAction, std::string("48 83 ?? ?? 0F B6 ?? 4C 8D ?? ?? ?? ?? ?? ?? 54"));

	/* Find reach ptrs */
	uintptr_t creativeReachSig = Mem::FindSig("f3 44 0f 5d 3d ? ? ? ? eb");
	uintptr_t survivalReachSig = Mem::FindSig("f3 0f 10 05 ? ? ? ? 44 0f 2f f8");
	assert(creativeReachSig != 0 && survivalReachSig != 0);
	uintptr_t creativeReachOffsetRef = creativeReachSig + 5;
	uintptr_t survivalReachOffsetRef = survivalReachSig + 4;
	uint32_t creativeReachOffset = *reinterpret_cast<uint32_t*>(creativeReachOffsetRef);
	uint32_t survivalReachOffset = *reinterpret_cast<uint32_t*>(survivalReachOffsetRef);
	auto* creativeReachPtr = reinterpret_cast<float*>(creativeReachOffsetRef + creativeReachOffset + sizeof(creativeReachOffset));
	auto* survivalReachPtr = reinterpret_cast<float*>(survivalReachOffsetRef + survivalReachOffset + sizeof(survivalReachOffset));

	//Unprotect the memory values
	DWORD oldProt = 0;
	VirtualProtect(creativeReachPtr, 4, PAGE_EXECUTE_READWRITE, &oldProt);
	VirtualProtect(survivalReachPtr, 4, PAGE_EXECUTE_READWRITE, &oldProt);

	this->addResult(SigTable::Global_creativeReachPtr, creativeReachPtr);
	this->addResult(SigTable::Global_reachPtr, survivalReachPtr);

	//this->addResult(SigTable::Global_decoupleRotations, "f3 0f 11 30 f3 0f 11 78 ? 49 8b ce");
}
