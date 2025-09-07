#include "lib/registry.h"
#include <iostream>

int main() {
    std::string rootKey = "SOFTWARE\\HPNC\\SneakerTest";
    std::string valueDword = "newValue";
    try {
        RegistryKey key(HKEY_LOCAL_MACHINE, rootKey , true);

        key.getDword(valueDword);

        key.setDword(valueDword, 0);
        std::cout << "The KEY: " << rootKey << "\\" << valueDword << " Exists: " << key.getDword(valueDword) << "\n";

        key.setString("Owner", "Admin");
        std::cout << "Owner: " << key.getString("Owner") << "\n";

        if (key.deleteValue("TempValue")) {
            std::cout << "TempValue deleted or didn't exist\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
