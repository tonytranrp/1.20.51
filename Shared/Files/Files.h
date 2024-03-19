#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Files {
	//The current module's path
	std::string GetModulePath(std::string module_name);
	//The directory of where a specificed module is located, "(Base)" for getting the .exe's home dir
	std::string GetModuleDir(std::string module_name);
	//The current execution directory
	std::string GetExecDir();
	//The current execution file
	std::string GetExecPath();
	//Directory to store data universally (no matter where injector is moved, this data is in the same spot)
	std::string GetUniformDir();

	HANDLE OpenFile(const std::string& file_path);
	std::pair<char*, size_t> ReadFile(HANDLE file_handle);
	void SaveFile(HANDLE file_handle, const char* content, size_t length);
	void SaveFile(HANDLE file_handle, const std::string& content);
	void CloseFile(HANDLE file_handle);

	//Fucking windows macro moment
#undef DeleteFile
	bool DeleteFile(std::string file_path);
};