#include "registry.h"
#include <sstream>
#include <vector>

RegistryKey::RegistryKey(HKEY root, const std::string& subKey, REGSAM accessbool,bool createIfMissing) {
    LONG result;
    if (createIfMissing) {
        result = RegCreateKeyExA(root,
                                 subKey.c_str(),
                                 0,
                                 NULL,
                                 REG_OPTION_NON_VOLATILE,
                                 KEY_READ | KEY_WRITE,
                                 NULL,
                                 &hKey,
                                 NULL);
    } else {
        result = RegOpenKeyExA(root, subKey.c_str(), 0, KEY_READ | KEY_WRITE, &hKey);
    }

    if (result != ERROR_SUCCESS) {
        std::ostringstream oss;
        oss << "Failed to open/create registry key: " << subKey << " (Error " << result << ")";
        throw std::runtime_error(oss.str());
    }
}

RegistryKey::~RegistryKey() {
    if (hKey) {
        RegCloseKey(hKey);
    }
}

RegistryKey::RegistryKey(RegistryKey&& other) noexcept : hKey(other.hKey) {
    other.hKey = nullptr;
}

RegistryKey& RegistryKey::operator=(RegistryKey&& other) noexcept {
    if (this != &other) {
        if (hKey) {
            RegCloseKey(hKey);
        }
        hKey = other.hKey;
        other.hKey = nullptr;
    }
    return *this;
}

// ----------------- DWORD -----------------
std::optional<DWORD> RegistryKey::getDword(const std::string& name) const {
    DWORD type = 0;
    DWORD data = 0;
    DWORD size = sizeof(data);
    LONG result = RegQueryValueExA(hKey, name.c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(&data), &size);

    if (result == ERROR_FILE_NOT_FOUND) {
        return std::nullopt; // explicitly missing
    }
    if (result != ERROR_SUCCESS) {
        std::ostringstream oss;
        oss << "Failed to read DWORD registry value '" << name << "' (Error " << result << ")";
        throw std::runtime_error(oss.str());
    }
    if (type != REG_DWORD) {
        std::ostringstream oss;
        oss << "Registry value '" << name << "' is not a DWORD (type=" << type << ")";
        throw std::runtime_error(oss.str());
    }

    return data;
}
//DWORD RegistryKey::getDword(const std::string& name, DWORD defaultVal) {
//DWORD type = 0, data = 0, size = sizeof(data);
//    LONG result = RegQueryValueExA(hKey, name.c_str(), NULL, &type, (LPBYTE)&data, &size);
//
//    if (result != ERROR_SUCCESS) {
//        // If you want missing keys to use default instead of throwing:
//        if (result == ERROR_FILE_NOT_FOUND)
//            return defaultVal;
//
//        std::ostringstream oss;
//        oss << "Failed to read DWORD value '" << name << "' (Error " << result << ")";
//        throw std::runtime_error(oss.str());
//    }
//
//    if (type != REG_DWORD) {
//        std::ostringstream oss;
//        oss << "Registry value '" << name << "' is not a DWORD (type=" << type << ")";
//        throw std::runtime_error(oss.str());
//    }
//
//    return data;
//}


void RegistryKey::setDword(const std::string& name, DWORD value) {
    LONG result = RegSetValueExA(hKey, name.c_str(), 0, REG_DWORD, (const BYTE*)&value, sizeof(value));
    if (result != ERROR_SUCCESS) {
        std::ostringstream oss;
        oss << "Failed to set DWORD value: " << name << " (Error " << result << ")";
        throw std::runtime_error(oss.str());
    }
}

// ----------------- STRING -----------------
std::string RegistryKey::getString(const std::string& name, const std::string& defaultVal) {
    DWORD type = 0, size = 0;
    LONG result = RegQueryValueExA(hKey, name.c_str(), NULL, &type, NULL, &size);
    if (result != ERROR_SUCCESS || type != REG_SZ) return defaultVal;

    std::vector<char> buffer(size);
    result = RegQueryValueExA(hKey, name.c_str(), NULL, NULL, (LPBYTE)buffer.data(), &size);
    if (result != ERROR_SUCCESS) return defaultVal;

    return std::string(buffer.data());
}

void RegistryKey::setString(const std::string& name, const std::string& value) {
    LONG result = RegSetValueExA(hKey, name.c_str(), 0, REG_SZ, (const BYTE*)value.c_str(),
                                 static_cast<DWORD>(value.size() + 1));
    if (result != ERROR_SUCCESS) {
        std::ostringstream oss;
        oss << "Failed to set string value: " << name << " (Error " << result << ")";
        throw std::runtime_error(oss.str());
    }
}

// ----------------- DELETE -----------------
bool RegistryKey::deleteValue(const std::string& name) {
    LONG result = RegDeleteValueA(hKey, name.c_str());
    return (result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND);
}
