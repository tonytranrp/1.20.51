#pragma once

#include "../IPatch.h"
#include <d3d12.h>

class SignalHook : public IPatch {
	static auto __fastcall signalCallback(ID3D12CommandQueue* queue, ID3D12Fence* fence, UINT64 value)->HRESULT;
public:
	static inline uintptr_t funcOriginal = 0;
	SignalHook();
	bool apply() override;
	bool manualCleanup() override;;
};