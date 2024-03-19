#ifndef NUVOLA_CLIENT_PATCHES_DXGI_PRESENTHOOK
#define NUVOLA_CLIENT_PATCHES_DXGI_PRESENTHOOK

#include "../IPatch.h"
#include <dxgi.h>
#include <dxgi1_4.h>

class PresentHook : public IPatch {
	static auto __fastcall presentCallback(IDXGISwapChain3* pSwapChain, int syncInterval, int flags) -> HRESULT;
public:
	static inline uintptr_t funcOriginal = 0;
	PresentHook();
	bool apply() override;
	bool manualCleanup() override;;
};


#endif /* NUVOLA_CLIENT_PATCHES_DXGI_PRESENTHOOK */
