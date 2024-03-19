#pragma once

#include "../../Bridge/Mob.h"

#include "../IPatch.h"

class ItemUseSlowdownHook : public IPatch
{
	static void __cdecl itemUseSlowdownCallback_1_20_51_1(void* dont_care, class StrictEntityContext* context, void*, void*);
public:
	static inline uintptr_t funcOriginal = 0;
	ItemUseSlowdownHook();
	bool apply() override;
};