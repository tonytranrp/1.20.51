#include "Registry.h"

void Registry::ReadValue(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text)
{

    if (value_buffer == nullptr) {
        throw std::exception("value_buffer must be a char[MAX_PATH]!");
        return;
    }

    HKEY hKey;
    LSTATUS status = RegOpenKeyExA(base_key, path.c_str(), 0, KEY_READ, &hKey);
    if (status != ERROR_SUCCESS) {
        printf("Failed to open registry key '%s'", path.c_str());
        throw std::exception("Failed to open registry key!");
        return;
    }

    DWORD dwLen = MAX_PATH;
    DWORD dwType = 0;
    status = RegQueryValueExA(hKey, key.c_str(), nullptr, &dwType, (BYTE*)value_buffer, &dwLen);
    if (status != ERROR_SUCCESS) {
        printf("Failed to read registry key '%s' in path '%s'", key.c_str(), path.c_str());
        throw std::exception("Failed to read registry key!");
        return;
    }

    status = RegCloseKey(hKey);
    if (status != ERROR_SUCCESS) {
        printf("Failed to close registry key '%s' in path '%s'", key.c_str(), path.c_str());
        throw std::exception("Failed to close registry key!");
    }

    printf("dwlen: %x", dwLen);
    if (is_text && dwLen <= MAX_PATH) {
        value_buffer[dwLen] = 0;
    }

    return;
}

std::string Registry::ReadString(HKEY base_key, std::string path, std::string key)
{
    char value_buffer[MAX_PATH];
    ReadValue(base_key, path, key, value_buffer);
    return std::string(value_buffer);
}

int Registry::ReadDword(HKEY base_key, std::string path, std::string key)
{
    int value_buffer;
    ReadValue(base_key, path, key, (char*)&value_buffer);
    return value_buffer;
}

void Registry::WriteValue(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text)
{
    if (value_buffer == nullptr) {
        throw std::exception("value_buffer must be a char[MAX_PATH]!");
        return;
    }

    HKEY hKey;
    LSTATUS status = RegOpenKeyExA(base_key, path.c_str(), 0, KEY_SET_VALUE, &hKey);
    if (status != ERROR_SUCCESS) {
        status = RegCreateKeyA(base_key, path.c_str(), &hKey);
        if (status != ERROR_SUCCESS) {
            throw std::exception("Failed to create a registry key");
            return;
        }
    }

    DWORD dwLen = is_text ? MAX_PATH : sizeof(int);
    DWORD dwType = is_text ? REG_SZ : REG_DWORD;
    status = RegSetValueExA(hKey, key == "(Default)" ? NULL : key.c_str(), 0, dwType, (BYTE*)value_buffer, dwLen);
    if (status != ERROR_SUCCESS) {
        printf("Failed to write registry key '%s' in path '%s'", key.c_str(), path.c_str());
        throw std::exception("Failed to write registry key!");
        return;
    }

    status = RegCloseKey(hKey);
    if (status != ERROR_SUCCESS) {
        printf("Failed to close registry key '%s' in path '%s'", key.c_str(), path.c_str());
        throw std::exception("Failed to close registry key!");
    }

    return;
}

void Registry::WriteString(HKEY base_key, std::string path, std::string key, std::string value)
{
    return WriteValue(base_key, path, key, (char*)value.c_str(), true);
}

void Registry::WriteDword(HKEY base_key, std::string path, std::string key, int value)
{
    return WriteValue(base_key, path, key, (char*)&value, false);
}

/*bool Registry::DeleteValue(HKEY base_key, std::string path, std::string key)
{
    HKEY hKey;
    LSTATUS status = RegOpenKeyExA(base_key, path.c_str(), 0, KEY_SET_VALUE | DELETE, &hKey);
    if (status != ERROR_SUCCESS) {
        status = RegDeleteKeyA(base_key, path.c_str());
        if (status != ERROR_SUCCESS) {
            printf("Failed to delete a registry key");
            return false;
        }
        return true;
    }
    else {
        printf("Failed to open a registry key to delete");
    }
    return false;
}*/

bool Registry::DeleteAll(HKEY base_key, std::string path)
{
    HKEY hKey;
    LSTATUS status = RegOpenKeyExA(base_key, path.c_str(), 0, KEY_SET_VALUE | DELETE | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hKey);
    if (status == ERROR_SUCCESS) {
        status = RegDeleteTreeA(base_key, path.c_str());
        if (status != ERROR_SUCCESS) {
            printf("Failed to delete a registry tree");
            return false;
        }

        status = RegCloseKey(hKey);
        if (status != ERROR_SUCCESS) {
            printf("Failed to close registry key in path '%s'", path.c_str());
            throw std::exception("Failed to close registry key!");
        }

        return true;
    }
    else {
        printf("Failed to open a registry tree to delete");
    }
    return false;
}