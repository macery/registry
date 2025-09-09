#pragma once
#include <windows.h>
#include <string>
//#include <vector>
//#include <stdexcept>
#include <optional>

class RegistryKey {

private:
    HKEY hKey;    

public:
    //RegistryKey(HKEY root, const std::string& subKey, bool createIfMissing = false);
    RegistryKey(HKEY root, const std::string& subKey, REGSAM access = KEY_READ, bool createIfMissing = false );
    ~RegistryKey();

    RegistryKey(const RegistryKey&) = delete;
    RegistryKey& operator = (const RegistryKey&& other) = delete;

    RegistryKey(RegistryKey&& other) noexcept;
    RegistryKey& operator=(RegistryKey&& other) noexcept;

    // DWORD operations
    //DWORD getDword(const std::string& name, DWORD defaultVal = 0);
    std::optional<DWORD> getDword(const std::string& name) const;
    void setDword(const std::string& name, DWORD value);

    // String operations
    std::string getString(const std::string& name, const std::string& defaultVal = "");
    void setString(const std::string& name, const std::string& value);

    // Delete value
    bool deleteValue(const std::string& name);


};
