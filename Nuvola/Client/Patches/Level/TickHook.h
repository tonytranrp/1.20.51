#pragma once

#include "../IPatch.h"
#include "../../Bridge/Level.h"

namespace _Level {
	class TickHook : public IPatch {
		static void __fastcall TickHookCallback_1_19_22(Level* level);
	public:
		static inline uintptr_t funcOriginal = 0;
		TickHook();
		bool apply() override;
	};
}