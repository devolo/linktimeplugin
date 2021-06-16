/**
 * @brief Link-time plug-ins demo program
 * @version 1.0.0
 * @author Wolfram Rösler
 * @date 2018-06-25
 * change:
 * 2021-05-25: exercise iterator interface (janos.vaczi@gmail.com)
 * @copyright MIT license
 */

#include <iostream>
#include <string>
#include <algorithm>
#include "linktimeplugin.hpp"

namespace {
    // Define the plug-in base class.
    // In a real application, this would be in a header file.
    DEFINE_PLUGIN_INTERFACE(PluginBase) {
    public:
        // Define the functions that are to be implemented by
        // the plug-ins. Must be pure virtual and public in the
        // base class. Can have any name and signature.
        virtual std::string name() = 0;
        virtual std::string sound() = 0;
    };

    // Define the first plug-in.
    // In a real application, this would be in its own .cpp
    // file, within an anonymous namespace (so the plug-in
    // isn't visible anywhere outside its own file).
    // Implements all plug-in functions (but the implementation
    // can be private in the plug-in class).
    class Cat : public PluginBase {
        std::string name() override {
            return "Cat";
        }

        std::string sound() override {
            return "Meow";
        }
    };
    REGISTER_PLUGIN(Cat);

    // Define the second plug-in.
    // Again, in a real application this would be in an
    // anonymous namespace within its own .cpp file.
    class Dog : public PluginBase {
        std::string name() override {
            return "Dog";
        }

        std::string sound() override {
            return "Woof";
        }
    };
    REGISTER_PLUGIN(Dog);

    // Define the third plug-in.
    class Bird : public PluginBase {
        std::string name() override {
            return "Bird";
        }

        std::string sound() override {
            return "Tweet";
        }
    };
    REGISTER_PLUGIN(Bird);
}

// Main function.
// Note that, in a real application, this will not be able
// to see any of the plug-in classes (Cat, Dog, ...), only
// the plug-in base class (PluginBase).
int main() {
    for (const auto animal : PluginBase::getPlugins()) {
        std::cout << animal->name() << ": " << animal->sound() << '\n';
    }
    std::cout << "again with iterators\n";
    for (auto it = PluginBase::begin(); it != PluginBase::end(); ++it) {
        std::cout << (*it)->name() << ": " << (*it)->sound() << '\n';
    };
    std::cout << "again with for_each\n";
    std::for_each(PluginBase::begin(), PluginBase::end(),
        [](PluginBase* x) {
            std::cout << x->name() << ": " << x->sound() << '\n';
        }
    );

    return EXIT_SUCCESS;
}
