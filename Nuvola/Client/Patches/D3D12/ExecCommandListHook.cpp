#include "ExecCommandListHook.h"

#include <kiero.h>

ID3D12CommandQueue* d3d12CommandQueue = nullptr;

void ExecCommandListHook::execCommandListCallback(ID3D12CommandQueue* pCommandQueue, int commandListCount, ID3D12CommandList** ppCommandList) {
	d3d12CommandQueue = pCommandQueue;
	return PLH::FnCast(funcOriginal, &execCommandListCallback)(pCommandQueue, commandListCount, ppCommandList);
}

ExecCommandListHook::ExecCommandListHook() : IPatch("D3D12::ExecuteCommandList") {

}

bool ExecCommandListHook::apply() {
	uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 54 : 119;
	return kiero::bind(index, (void**)&funcOriginal, execCommandListCallback) == kiero::Status::Success;
}

bool ExecCommandListHook::manualCleanup() {
	cleaned = true;
	uint16_t index = kiero::getRenderType() == kiero::RenderType::D3D12 ? 54 : 119;
	kiero::unbind(index);
	return true;
}