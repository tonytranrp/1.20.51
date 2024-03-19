#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <vector>

bool LoadLib(HANDLE hProc, std::string dllFile);
bool ManualMap(HANDLE hProc, std::vector<uint8_t> dllBytes, bool ClearHeader, bool ClearNonNeededSections, bool AdjustProtections, bool SEHExceptionSupport, DWORD fdwReason, LPVOID lpReserved);