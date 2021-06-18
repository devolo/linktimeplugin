/**
 * @brief Include file for link-time plug-ins demo program
 * @author Wolfram Rösler
 * @date 2021-06-18
 * @copyright MIT license
 */

#pragma once

#include <string>
#include "linktimeplugin.hpp"

// Define the base class for our plug-ins
class Animal {
public:
    // Make the class known to the registrar
    using Base = Animal;

    // Define the functions that are to be implemented by
    // the plug-ins. Must be pure virtual and public.
    // Can have any name and signature.
    virtual std::string name() = 0;
    virtual std::string sound() = 0;
};
