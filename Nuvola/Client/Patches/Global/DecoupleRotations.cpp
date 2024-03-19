#include "DecoupleRotations.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DecoupleRotations::DecoupleRotations() : IPatch("Global::decoupleRotations")
{
	this->addSignature(SigTable::Global_decoupleRotations); //1.20.31
}

bool DecoupleRotations::apply()
{
	uintptr_t hookAddr = this->scanSigs();
	if (hookAddr == 0) {
		return false;
	}
	instructionPtr = reinterpret_cast<uint32_t*>(hookAddr);

	DWORD oldProt = 0;
	VirtualProtect(instructionPtr, 4, PAGE_EXECUTE_READWRITE, &oldProt);

	instructionOriginal = *instructionPtr;
	*instructionPtr = 0x90909090;

	return true;
}

bool DecoupleRotations::manualCleanup()
{
	*instructionPtr = instructionOriginal;
	this->cleaned = true;
	return true;
}