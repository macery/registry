
#include <iostream>
#include <string>
#include "lib/registry.h"  // your library header

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: get-dword.exe <RegistryKeyPath> <ValueName>\n";
        return 1;  // error code for bad args
    }

    std::string keyPath = argv[1];
    std::string valueName = argv[2];
    try {
        RegistryKey key(HKEY_LOCAL_MACHINE, keyPath, false); // false = don't create
        //DWORD val = key.getDword(valueName);
        auto val = key.getDword(valueName);
        if (!val){
            std::cout << "Value does not exist\n";
        } else {
        std::cout << *val << "\n";  // value to stdout
        return 0;  // success
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;  // error code for library failure
    }
}
