#include <iostream>
#include <string>
#include "lib/registry.h"
#include <cstdlib>
#include <stdexcept>

// Parses the root from the full key path and returns the HKEY constant
HKEY parseRootKey(std::string& path) {
    auto pos = path.find("\\");
    std::string firstPart, subPath;

    if (pos != std::string::npos) {
        firstPart = path.substr(0, pos);
        subPath = path.substr(pos + 1);
    } else {
        firstPart = "";  // No root prefix
        subPath = path;
    }

    path = subPath;  // strip root from path for RegistryKey

    if (firstPart.empty()) return HKEY_LOCAL_MACHINE;
    if (firstPart == "HKLM" || firstPart == "HKEY_LOCAL_MACHINE") return HKEY_LOCAL_MACHINE;
    if (firstPart == "HKCU" || firstPart == "HKEY_CURRENT_USER") return HKEY_CURRENT_USER;
    if (firstPart == "HKCR" || firstPart == "HKEY_CLASSES_ROOT") return HKEY_CLASSES_ROOT;
    if (firstPart == "HKU"  || firstPart == "HKEY_USERS") return HKEY_USERS;
    if (firstPart == "HKCC" || firstPart == "HKEY_CURRENT_CONFIG") return HKEY_CURRENT_CONFIG;

    throw std::runtime_error("Unknown root key: " + firstPart);
}

//DWORD convertToDWORD(){

//}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: regtool <command> <RegistryKeyPath> [ValueName] [Value]\n";
        return 1;
    }
    std::string command = argv[1];
    std::string keyPath = argv[2];
    HKEY root;

    try {
        root = parseRootKey(keyPath);


        if (command == "get-dword") {
            if (argc != 3) throw std::runtime_error("Usage: get-dword <RegistryKeyPath> <ValueName>");
            RegistryKey key(root, keyPath, false);  // get: don't create
            DWORD val = key.getDword(argv[3]);
            std::cout << val << "\n";
        }
        else if (command == "set-dword") {
            if (argc != 4) throw std::runtime_error("Usage: set-dword <RegistryKeyPath> <ValueName> <Value>");
            RegistryKey key(root, keyPath, true);   // set: create if missing
            DWORD val = std::strtoul(argv[4], nullptr, 0);
            key.setDword(argv[3], val);
        }
        else if (command == "get-string") {
            if (argc != 3) throw std::runtime_error("Usage: get-string <RegistryKeyPath> <ValueName>");
            RegistryKey key(root, keyPath, false);
            std::cout << key.getString(argv[3]) << "\n";
        }
        else if (command == "set-string") {
            if (argc != 4) throw std::runtime_error("Usage: set-string <RegistryKeyPath> <ValueName> <Value>");
            RegistryKey key(root, keyPath, true);
            key.setString(argv[3], argv[4]);
        }
        else {
            throw std::runtime_error("Unknown command: " + command);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }
return 0;
}
