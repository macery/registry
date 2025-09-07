#include <iostream>
#include <string>
#include <stdexcept>
#include "lib/registry.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: get-string.exe <RegistryKeyPath> <ValueName>\n";
        return 1;
    }

    std::string keyPath = argv[1];
    std::string valueName = argv[2];

    try {
        RegistryKey key(HKEY_LOCAL_MACHINE, keyPath, false);
        std::string value = key.getString(valueName, "<not found>");
        std::cout << valueName << ": " << value << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }

    return 0;
}
