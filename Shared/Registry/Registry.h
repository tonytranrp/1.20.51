#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Registry {
	void ReadValue(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text = false);
	std::string ReadString(HKEY base_key, std::string path, std::string key);
	int ReadDword(HKEY base_key, std::string path, std::string key);

	void WriteValue(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text = false);
	void WriteString(HKEY base_key, std::string path, std::string key, std::string value);
	void WriteDword(HKEY base_key, std::string path, std::string key, int value);

	//bool DeleteValue(HKEY base_key, std::string path, std::string key);
	bool DeleteAll(HKEY base_key, std::string path);
};