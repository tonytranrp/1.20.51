#include "ProcUtil.h"

#include <Windows.h>
#include <tlhelp32.h>

void ProcUtil::ForEachProcess(std::function<void(std::string, size_t)> callback) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            callback(entry.szExeFile, entry.th32ProcessID);
        }
    }

    CloseHandle(snapshot);
}

size_t ProcUtil::FindProcID(std::string name) {
    size_t theId = 0;
    ForEachProcess([name, &theId](std::string procName, size_t procId) {
        if (procName.find(name) != std::string::npos) {
            theId = procId;
        }
    });
    return theId;
}