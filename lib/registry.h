#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>

class RegistryKey {
public:
    RegistryKey(HKEY root, const std::string& subKey, bool createIfMissing = false);
    ~RegistryKey();

    // DWORD operations
    DWORD getDword(const std::string& name, DWORD defaultVal = 0);
    void setDword(const std::string& name, DWORD value);

    // String operations
    std::string getString(const std::string& name, const std::string& defaultVal = "");
    void setString(const std::string& name, const std::string& value);

    // Delete value
    bool deleteValue(const std::string& name);

private:
    HKEY hKey = nullptr;
};
