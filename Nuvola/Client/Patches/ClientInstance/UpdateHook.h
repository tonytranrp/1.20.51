#pragma once

#include "../../Bridge/Nuvola/Bridge.h"
#include "../../Features/Modules/ModuleManager.h"
#include "../IPatch.h"

class UpdateHook : public IPatch {
	static void __fastcall clientInstanceCallback_1_20_12_1(class ClientInstance* theInstance, bool param_1);
public:
	static inline uintptr_t funcOriginal = 0;
	UpdateHook();
	bool apply() override;
};