/**
 * @brief Link-time plug-ins demo program
 * @author Wolfram Rösler
 * @date 2018-06-25
 * @copyright MIT license
 */

#include <iostream>
#include "demo.hpp"

int main() {
    for (const auto animal : linktimeplugin::plugins<Animal>()) {
        std::cout << animal->name() << ": " << animal->sound() << '\n';
    }

    return EXIT_SUCCESS;
}
