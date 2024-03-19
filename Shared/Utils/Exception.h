#pragma once

#include <Windows.h>

namespace Exception {
	LONG WINAPI exceptionHandler(_In_ struct _EXCEPTION_POINTERS* ExceptionInfo);
};