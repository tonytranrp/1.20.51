#ifndef NUVOLA_CLIENT_PATCHES_D3D12_EXECCOMMANDLISTHOOK
#define NUVOLA_CLIENT_PATCHES_D3D12_EXECCOMMANDLISTHOOK

#include "../IPatch.h"
#include <d3d12.h>

class ExecCommandListHook : public IPatch {
	static auto __fastcall execCommandListCallback(ID3D12CommandQueue* pCommandQueue, int commandListCount, ID3D12CommandList** pCommandList) -> void;
public:
	static inline uintptr_t funcOriginal = 0;
	ExecCommandListHook();
	bool apply() override;
	bool manualCleanup() override;;
};

#endif /* NUVOLA_CLIENT_PATCHES_D3D12_EXECCOMMANDLISTHOOK */