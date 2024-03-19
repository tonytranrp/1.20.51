#include "ResizeBuffersHook.h"

#include <kiero.h>
#include <d2d1_3.h>

#include <Graphics/imfx.h>

auto __fastcall ResizeBuffersHook::resizeBuffersCallback(IDXGISwapChain* pSwapChain, int bufferCount, int width, int height, DXGI_FORMAT newFormat, int swapChainFlags) -> HRESULT {
	ImFX::CleanupFX();
	return PLH::FnCast(funcOriginal, &resizeBuffersCallback)(pSwapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

ResizeBuffersHook::ResizeBuffersHook() : IPatch("DXGI::ResizeBuffers") {
}

bool ResizeBuffersHook::apply() {
	uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 145 : 13;
	return kiero::bind(index, (void**)&funcOriginal, resizeBuffersCallback) == kiero::Status::Success;
}

auto ResizeBuffersHook::manualCleanup() -> bool {
	cleaned = true;
	uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 145 : 13;
	kiero::unbind(index);
	return true;
}