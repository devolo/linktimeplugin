/**
 * @brief Link-time plug-ins demo program: Demo plug-in
 * @author Wolfram Rösler
 * @date 2021-06-18
 * @copyright MIT license
 */

#include "demo.hpp"

namespace {
    class Bird : public Animal {
        std::string name() override {
            return "Bird";
        }

        std::string sound() override {
            return "Tweet";
        }
    };

    REGISTER_PLUGIN(Bird);
}
