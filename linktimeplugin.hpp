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
    template<typename BASE>
    class RegistrarBase {
    public:
        // Ctor. Adds this object to the list of registrars.
        RegistrarBase() noexcept {
            registrars().push_back(this);
        }

        // Rule of 5
        virtual ~RegistrarBase() = default;
        RegistrarBase(const RegistrarBase&) = delete;
        RegistrarBase(RegistrarBase&&) = delete;
        void operator=(const RegistrarBase&) = delete;
        void operator=(RegistrarBase&&) = delete;

        // Implemented by the derived registrar class.
        virtual BASE& operator()() = 0;

        typedef std::vector<BASE*> PluginColl;
        typedef std::vector<RegistrarBase<BASE>*> RegistarColl;
        
        // Returns all registrars.
        static PluginColl plugins() {
            PluginColl ret;
            for(auto r : registrars()) {
                ret.push_back(&(*r)());
            }
            return ret;
        }
        // Use (*begin())() to get BASE object!
        static auto begin() {
            return registrars().begin();
        }
        
        static auto end() {
            return registrars().end();
        }
        
    private:
        static RegistarColl& registrars() {
            static RegistarColl regs;
            return regs;
        }
    };

    /*
     * Derived registrar class.
     * PLUGIN is the plug-in class (derived from the plug-in base class).
     */
    template<typename PLUGIN>
    class Registrar : public RegistrarBase<typename PLUGIN::Base> {
        PLUGIN plugin_;

        typename PLUGIN::Base& operator()() override {
            return plugin_;
        }
    };

    /**
     * Get pointers to instances of all registered plug-in classes.
     *
     * T is the plug-in base class.
     *
     * Example: (MyBase is the plug-in base class, DoSomething is a pure
     * virtual function in the plug-in base class, implemented by the
     * derived plug-in classes)
     *
     *      for (auto& p : linktimeplugin::plugins<MyBase>()) {
     *          p->DoSomething();
     *      }
     */
    template<typename T>
    std::vector<T*> plugins() {
        return RegistrarBase<T>::plugins();
    }
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
#define REGISTER_PLUGIN(x) static linktimeplugin::Registrar<x> x##registrar
