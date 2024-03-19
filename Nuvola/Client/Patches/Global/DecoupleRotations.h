#pragma once

#include "../IPatch.h"

//Patch to decouple camera rotations from the player

class DecoupleRotations : public IPatch
{
	uint32_t* instructionPtr = nullptr;
	uint32_t instructionOriginal = 0;
public:
	DecoupleRotations();
	bool apply() override;
	bool manualCleanup() override;
};