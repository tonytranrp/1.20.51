#include "Injector.h"

bool LoadLib(HANDLE hProc, std::string dllFile)
{
	char out_buf[MAX_PATH];
	DWORD path_size = GetFullPathNameA(dllFile.c_str(), MAX_PATH, out_buf, NULL);

	std::string path = out_buf;

	LPVOID remote_path = VirtualAllocEx(hProc, nullptr, path.length(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remote_path) {
		printf("Couldn't allocate memory in target process!");
		return false;
	}
	SIZE_T written_ct;
	if (!WriteProcessMemory(hProc, remote_path, path.c_str(), path.length(), &written_ct)) {
		printf("Couldn't write the dll filepath to target process memory!");
		printf("Last error: %x", GetLastError());
		return false;
	}
	HMODULE kernel_handle = GetModuleHandleA("kernel32.dll");
	if (!kernel_handle) {
		printf("Couldn't find kernel32.dll in target process!");
		return false;
	}
	FARPROC loadlib_addr = GetProcAddress(kernel_handle, "LoadLibraryA");
	if (!written_ct) {
		printf("Failed to find LoadLibraryA function!");
		return false;
	}
	HANDLE remote_thread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)loadlib_addr, remote_path, 0, NULL);
	if (!remote_thread) {
		printf("Failed to create the remote thread!");
		return false;
	}
	DWORD result = WaitForSingleObject(remote_thread, 10 * 1000);
	if (result == WAIT_ABANDONED || result == WAIT_TIMEOUT) {
		printf("The thread was either abandoned or timed out, and could not execute the code.");
		return false;
	}
	if (!VirtualFreeEx(hProc, remote_path, 0, MEM_RELEASE)) {
		printf("The temporary injection memory couldn't be freed, so it leaked approximately %llx bytes", path.length());
	}
	return true;
}