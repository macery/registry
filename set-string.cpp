#include <iostream>
#include <string>
#include <stdexcept>
#include "lib/registry.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: set-string.exe <RegistryKeyPath> <ValueName> <StringValue>\n";
        return 1;
    }

    std::string keyPath = argv[1];
    std::string valueName = argv[2];
    std::string value = argv[3];

    try {
        RegistryKey key(HKEY_LOCAL_MACHINE, keyPath, true);
        std::string current = key.getString(valueName, "<__MISSING__>");
    if (current == "<__MISSING__>") {
        // Key/value didn’t exist → create
        key.setString(valueName, value);
        std::cout << "Value created: " << valueName << " = \"" << value << "\"\n";
    } else if (current != value) {
        // Key/value exists but differs → update
        key.setString(valueName, value);
        std::cout << "Value updated: " << valueName << " from \"" << current
              << "\" to \"" << value << "\"\n";
    }    else {
        // Already correct → no change
        std::cout << "No change needed: " << valueName << " = \"" << value << "\"\n";
}
        // Optional verification
        std::string afterSet = key.getString(valueName);
        if (afterSet != value) {
            std::cerr << "Value for " << valueName << " not set correctly!\n";
            std::cerr << "Expected: " << value << " Got: " << afterSet << "\n";
            return 2;
        }

        std::cout << "Successfully set " << valueName << " to \"" << value
                  << "\" under " << keyPath << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }

    return 0;
}
