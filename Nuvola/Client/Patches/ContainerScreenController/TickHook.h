#pragma once

#include "../IPatch.h"

class ContainerScreenController;
namespace _ContainerScreenController {
	class TickHook : public IPatch {
		static uint32_t __fastcall TickCallback_1_20_0(ContainerScreenController* controller);
	public:
		static inline uintptr_t funcOriginal = 0;
		TickHook();
		bool apply() override;
	};
}