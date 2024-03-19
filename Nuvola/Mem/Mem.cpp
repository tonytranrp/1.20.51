#include "Mem.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#include <Windows.h>
#include <Psapi.h>

#include <libhat/Scanner.hpp>

uintptr_t Mem::FindMLvlPtr(uintptr_t baseAddr, std::vector<unsigned int> offsets){
    uintptr_t addr = baseAddr;
	for (int I = 0; I < offsets.size(); I++) {
		addr = *(uintptr_t*)(addr);
		if ((uintptr_t*)(addr) == nullptr)
			return addr;
		addr += offsets[I];
	}
	return addr;
}

uintptr_t Mem::FindSig(std::string_view pattern)
{
    auto sig = hat::parse_signature(pattern);
    assert(sig.has_value());
    auto result = hat::find_pattern(sig.value(), ".text");
    return result.has_result() ? reinterpret_cast<uintptr_t>(result.get()) : NULL;
}

uintptr_t Mem::FindSig(uintptr_t rangeStart, uintptr_t rangeEnd, std::string_view pattern)
{
    auto sig = hat::parse_signature(pattern);
    assert(sig.has_value());

    auto result = hat::find_pattern(
        reinterpret_cast<const std::byte*>(rangeStart),
        reinterpret_cast<const std::byte*>(rangeEnd),
        sig.value()
    );
    return result.has_result() ? reinterpret_cast<uintptr_t>(result.get()) : NULL;
}