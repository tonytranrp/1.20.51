#include "SignalHook.h"

#include <kiero.h>

auto __fastcall SignalHook::signalCallback(ID3D12CommandQueue* queue, ID3D12Fence* fence, UINT64 value) -> HRESULT {
	return PLH::FnCast(funcOriginal, &signalCallback)(queue, fence, value);
}

SignalHook::SignalHook() : IPatch("D3D12::Signal")
{
}

bool SignalHook::apply()
{
	return kiero::bind(58, (void**)&funcOriginal, signalCallback) == kiero::Status::Success;
}

bool SignalHook::manualCleanup()
{
	cleaned = true;
	kiero::unbind(58);
	return true;
}