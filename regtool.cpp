#include <iostream>
#include "registry.h"
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: regtool <command> [args...]\n";
        std::cerr << "Commands:\n";
        std::cerr << "  get-dword <RegistryKeyPath> <ValueName>\n";
        std::cerr << "  set-dword <RegistryKeyPath> <ValueName> <Value>\n";
        std::cerr << "  get-string <RegistryKeyPath> <ValueName>\n";
        std::cerr << "  set-string <RegistryKeyPath> <ValueName> <Value>\n";
        return 1;
    }

    std::string command = argv[1];

    try {
        if (command == "get-dword") {
            if (argc != 4) { /* usage error */ }
            RegistryKey key(HKEY_LOCAL_MACHINE, argv[2], false);
            DWORD val = key.getDword(argv[3]);
            std::cout << val << "\n";
        }
        else if (command == "set-dword") {
            if (argc != 5) { /* usage error */ }
            RegistryKey key(HKEY_LOCAL_MACHINE, argv[2], true);
            DWORD val = std::strtoul(argv[4], nullptr, 0);
            key.setDword(argv[3], val);
        }
        else if (command == "get-string") {
            if (argc != 4) { /* usage error */ }
            RegistryKey key(HKEY_LOCAL_MACHINE, argv[2], false);
            std::cout << key.getString(argv[3]) << "\n";
        }
        else if (command == "set-string") {
            if (argc != 5) { /* usage error */ }
            RegistryKey key(HKEY_LOCAL_MACHINE, argv[2], true);
            key.setString(argv[3], argv[4]);
        }
        else {
            std::cerr << "Unknown command: " << command << "\n";
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }

    return 0;
}
