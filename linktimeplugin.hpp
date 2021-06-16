/**
 * @brief Link-time plug-in management
 * @version 1.0.1
 * @author Wolfram Rösler
 * @date 2018-06-25
 * change:
 * 2021-05-25: registrars vector to static and added iterator interface (janos.vaczi@gmail.com)
 * @copyright MIT license
 */

#pragma once

#include <vector>

/**
 * Link-time plug-in management.
 *
 * Usage:
 *
 *  1. Define a base class for your plug-ins.
 *  2. In this plug-in base class, add "public: using Base=x",
 *     where x is the name of the plug-in base class.
 *  3. For every plug-in, derive a class from the base class.
 *  4. For every such class, invoke REGISTER_PLUGIN(x), where x is the
 *     name of the derived plug-in class.
 *  5. To retrieve a list of all plug-ins, invoke linktimeplugin::plugins<x>(),
 *     where x is the name of the plug-in base class. This function
 *     returns a pointer to an instance of every plug-in class.
 *  6. Alternatively iterate the RegistrarBase objects with begin()/end() iterators.
 *     Don't forget to downcast like (*it)()
 */
namespace linktimeplugin {
    /*
     * Base class for plug-in registrars. A registrar is an intermediate
     * class that manages the registration of one plug-inclass (which
     * is derived from the common plug-in base class).
     */
    template<typename Plugin>
    class RegistrarBase {
    public:
        // Ctor. Adds this object to the list of registrars.
        RegistrarBase() noexcept try {
            registerPlugin(this);
        } catch (...) {            
        }

        // Rule of 5
        virtual ~RegistrarBase() = default;
        RegistrarBase(const RegistrarBase&) = delete;
        RegistrarBase(RegistrarBase&&) = delete;
        void operator=(const RegistrarBase&) = delete;
        void operator=(RegistrarBase&&) = delete;

        typedef std::vector<Plugin*> Collection;
        typedef typename Collection::const_iterator const_iterator;
        
        // provide only const data for user
        static const_iterator begin() { return registrars().begin(); }
        static const_iterator end() { return registrars().end(); }

        static const Collection& getPlugins() { return registrars(); }

    private:
        void registerPlugin(RegistrarBase<Plugin>* reg) {
            registrars().push_back(static_cast<Plugin*>(reg));
        }

        static Collection& registrars() {
            static Collection singleton;
            return singleton;
        }
    };
}

/**
 * Register one plug-in class.
 * Use this once for every plug-in class that's derived from the
 * plug-in base class.
 *
 * x is the name of the derived plug-in class.
 *
 * Example:
 *
 *      // Base class
 *      class PluginBase {
 *      public:
 *          using Base = PluginBase;
 *          virtual void DoSomething() = 0;
 *      };
 *
 *      // Plug-in class
 *      class Plugin: public PluginBase {
 *          void DoSomething() override { ... }
 *      };
 *
 *      // Register the plug-in class
 *      REGISTER_PLUGIN(Plugin);
 */
//#define REGISTER_PLUGIN(x) static linktimeplugin::Registrar<x> x##registrar
// REGISTER_PLUGIN clearly indicates intent
#define REGISTER_PLUGIN(x) static x x##Instance

// DEFINE_PLUGIN_INTERFACE avoids repetition of plugin class name
#define DEFINE_PLUGIN_INTERFACE(x) class x : public linktimeplugin::RegistrarBase<x>
