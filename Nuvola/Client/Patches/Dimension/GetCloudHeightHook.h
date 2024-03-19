#pragma once

#include "../IPatch.h"

#include "../../Bridge/Dimension.h"
#include "../../Bridge/Types.h"

class GetCloudHeightHook : public IPatch
{
	static int __fastcall GetCloudHeightCallback_1_20_40(const Dimension* self);
public:
	static inline uintptr_t funcOriginal = 0;
	GetCloudHeightHook();
	auto apply() -> bool override;
};