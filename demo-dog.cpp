/**
 * @brief Link-time plug-ins demo program: Demo plug-in
 * @author Wolfram Rösler
 * @date 2021-06-18
 * @copyright MIT license
 */

#include "demo.hpp"

namespace {
    class Dog : public Animal {
        std::string name() override {
            return "Dog";
        }

        std::string sound() override {
            return "Woof";
        }
    };

    REGISTER_PLUGIN(Dog);
}
