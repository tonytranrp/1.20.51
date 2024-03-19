#include "Files.h"

#include <Utils/Utils.h>

#include <filesystem>
#include <ShlObj_core.h>

std::string Files::GetModulePath(std::string module_name)
{
	HMODULE module_handle = GetModuleHandleA(module_name == "(Base)" ? NULL : module_name.c_str());
	char path_buffer[MAX_PATH];
	GetModuleFileNameA(module_handle, path_buffer, MAX_PATH);
	return path_buffer;
}

std::string Files::GetModuleDir(std::string module_name)
{
	std::filesystem::path modulePath = GetModulePath(module_name);
	std::filesystem::path parentDir = modulePath.parent_path();
	return parentDir.string();
}

std::string Files::GetExecDir()
{
	return GetModuleDir("(Base)");
}

std::string Files::GetExecPath()
{
	return GetModulePath("(Base)");
}

std::string Files::GetUniformDir()
{
	const std::string& dataDir = Utils::GetRoamingState() + "\\Nuvola";

	if (CreateDirectoryA(dataDir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		//ilog("Uniform directory created/already exists");
	}
	else
	{
		//ilog(LogLevel::Error, "Failed to create uniform directory, error: %x", GetLastError());
		return "";
	}

	return dataDir;
}

HANDLE Files::OpenFile(const std::string& file_path)
{
	HANDLE hAccFile = CreateFileA(file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hAccFile == INVALID_HANDLE_VALUE) {
		auto last_error = GetLastError();
		//ilog(LogLevel::Error, "Failed to open %s file. Error: %x", file_path.c_str(), last_error);
		return nullptr;
	}

	return hAccFile;
}

std::pair<char*, size_t> Files::ReadFile(HANDLE file_handle)
{
	int file_size = GetFileSize(file_handle, NULL);
	char* buffer = (char*)malloc(file_size);
	if (buffer == nullptr) {
		printf("Couldn't allocate memory for reading %p file", file_handle);
		__debugbreak();
		//throw std::exception("Failed to allocate adequate buffer");
	}
	if (!::ReadFile(file_handle, buffer, file_size, NULL, NULL)) {
		int last_error = GetLastError();
		printf("Failed to read %p file. Error: %x", file_handle, last_error);
		__debugbreak();
		//throw std::exception("Failed to read file");
	}

	return std::pair<char*, size_t>(buffer, file_size);
}

void Files::SaveFile(HANDLE file_handle, const char* content, size_t length) {
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);
	WriteFile(file_handle, content, length, NULL, NULL);
	SetEndOfFile(file_handle);
}

void Files::SaveFile(HANDLE file_handle, const std::string& content)
{
	Files::SaveFile(file_handle, content.c_str(), content.size());
}

void Files::CloseFile(HANDLE file_handle)
{
	CloseHandle(file_handle);
}

bool Files::DeleteFile(std::string file_path) {
	return DeleteFileA(file_path.c_str());
}