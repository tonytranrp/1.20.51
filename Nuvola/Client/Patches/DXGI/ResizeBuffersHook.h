#ifndef NUVOLA_CLIENT_PATCHES_DXGI_RESIZEBUFFERHOOK
#define NUVOLA_CLIENT_PATCHES_DXGI_RESIZEBUFFERHOOK

#include "../IPatch.h"
#include <dxgi.h>

class ResizeBuffersHook : public IPatch {
	static auto __fastcall resizeBuffersCallback(IDXGISwapChain* pSwapChain, int bufferCount, int width, int height, DXGI_FORMAT newFormat, int swapChainFlags) -> HRESULT;
public:
	static inline uintptr_t funcOriginal = 0;
	ResizeBuffersHook();
	bool apply() override;
	bool manualCleanup() override;;
};

#endif /* NUVOLA_CLIENT_PATCHES_DXGI_RESIZEBUFFERHOOK */
