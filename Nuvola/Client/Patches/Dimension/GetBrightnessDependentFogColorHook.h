#pragma once

#include "../IPatch.h"

#include "../../Bridge/Dimension.h"
#include "../../Bridge/Types.h"

#include <Math/Math.h>

class GetBrightnessDependentFogColorHook : public IPatch
{
	static ColorF* __fastcall getBrightnessDependentFogColorCallback_1_20_12_1(const Dimension* self, ColorF* mceColor /* mce::Color idk if in sdk*/, const __int64 arg3, const float arg4 /*idk im not very good at decompile*/);
public:
	static inline uintptr_t funcOriginal = 0;
	GetBrightnessDependentFogColorHook();
	auto apply() -> bool override;
};