#include <iostream>
#include <string>
#include "lib/registry.h"  // your library header
#include <cstdlib>      // for std::strtoul
#include <limits>       // for std::numeric_limits

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: set-dword.exe <RegistryKeyPath> <ValueName> <Value>\n";
        return 1;  // error code for bad args
    }
    std::string keyPath = argv[1];
    std::string valueDword = argv[2];
    
    // convert third argument to DWORD
    char* endPtr = nullptr;
    unsigned long val = std::strtoul(argv[3], &endPtr, 0); // base 0 allows 0x... hex
    if (*endPtr != '\0') {
        std::cerr << "Invalid DWORD value: " << argv[3] << "\n";
        return 1;
    }
    DWORD value = static_cast<DWORD>(val);

    RegistryKey key(HKEY_LOCAL_MACHINE, keyPath , true);
    try {
       auto current = key.getDword(valueDword );
    if (!current) {
      // Key/value didn’t exist → create
         key.setDword(valueDword, value);
         std::cout << "Value created: " << valueDword << " = " << value << "\n";
}   else if (current != value) {
      // Key/value exists but differs → update
        key.setDword(valueDword, value);
        std::cout << "Value updated: " << valueDword << " from " << *current
              << " to " << value << "\n";
}   else {
    // Already correct → no change
    std::cout << "No change needed: " << valueDword << " = " << value << "\n";
}
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;  // error code for library failure
    }
    auto afterSet = key.getDword(valueDword);
    if (!afterSet){
        std::cerr << "Values still doesn't exist!";
    }
    if (afterSet != value) {
        std::cerr << "Value for " << valueDword << " Not set correctly!\n";
        std::cerr << "Expected: " << value << " Got: " << *afterSet << "\n";
        return 2;
    }
return 0;
}