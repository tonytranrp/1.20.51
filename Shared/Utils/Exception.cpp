#include "Exception.h"

#include <vector>
#include <string>
#include <DbgHelp.h>

#include "../Files/Files.h"
#include "Utils.h"

LONG WINAPI Exception::exceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
    HANDLE currentProcess = GetCurrentProcess();
    DWORD currentPID = GetCurrentProcessId();
    HANDLE dumpFile = Files::OpenFile(Files::GetUniformDir() + "/nuvola_crash.dmp");
    MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
        MiniDumpWithFullMemory |
        MiniDumpWithHandleData |
        MiniDumpWithUnloadedModules |
        MiniDumpWithThreadInfo |
        MiniDumpIgnoreInaccessibleMemory |
        MiniDumpWithIptTrace);
    DWORD threadId = GetCurrentThreadId();
    MINIDUMP_EXCEPTION_INFORMATION dumpExceptionInfo = {
        threadId,
        ExceptionInfo,
        FALSE
    };
    bool dumpMade = MiniDumpWriteDump(currentProcess, currentPID, dumpFile, dumpType, &dumpExceptionInfo, NULL, NULL);
    return 0;
}