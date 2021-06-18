/**
 * @brief Link-time plug-ins demo program: Demo plug-in
 * @author Wolfram Rösler
 * @date 2021-06-18
 * @copyright MIT license
 */

#include "demo.hpp"

namespace {
    class Cat : public Animal {
        std::string name() override {
            return "Cat";
        }

        std::string sound() override {
            return "Meow";
        }
    };

    REGISTER_PLUGIN(Cat);
}
