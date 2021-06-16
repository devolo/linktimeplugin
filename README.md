# Link-Time Plug-In Management

C++11 header-only library for registration and management of link-time plug-ins.

## What is it?

In order to achieve a modular structure, many applications use a plug-in based architecture that cleanly separates the core logic from the implementation of individual drivers/handlers that do the actual work. For example:

* A web server has a plug-in for every URI path.
* A unit test framework has a plug-in for every test case.
* A command line application has a plug-in for every subcommand.
* An image viewer has a plug-in for every image file type.
* An SNMP agent has a plug-in for every OID.

There are many possible implementations of such a plug-in architecture:

* Plug-ins may be shared libraries (.so/.dll files) which the application loads
* Plug-ins may be executables to which the application talks via stdin/stdout
* Plug-ins may be scripts executed by an interpreter that's built into the application

All these architectures allow plug-ins to be added or removed dynamically at run-time, without having to rebuild, re-install, or (sometimes) even restart the application. We'll call these *run-time plug-ins*. Run-time plug-ins are not what this repo is about.

This repo is about *link-time plug-ins*, which are plug-ins that are compiled and linked into the application. Adding or removing plug-ins requires the application to be rebuilt, so there are no run-time dependencies on external files. Link-time plug-ins are as independent and isolated from the application core as run-time plugins:

* Link-time plug-ins can be added to the application simply by adding their .cpp file name to the build configuration (e. g. cmake file).
* Link-time plug-ins are self-contained within their .cpp files and don't export any implementation details.
* Link-time plug-ins are registered from within their .cpp file. No external list of plug-ins needs to be maintained.
* Link-time plug-ins don't know anything about the application that invokes them.
* The application doesn't know anything about the plug-ins beyond their predefined API.
* The application can iterate over existing plug-ins and invoke functions in the plug-ins.

## What's the benefit?

If all plug-ins are linked into the executable, what's the point of having a plug-in architecture in the first place?

The answer is isolation. Imagine a git-like command line program that executes subcommands like this:

```cpp
int main(int argc, char** argv) {
    ...

    if (strcmp(argv[1], "pull")==0) {
        doPull(argc, argv);
    } else if (strcmp(argv[1], "commit")==0) {
        doCommit(argc, argv);
    } else if (strcmp(argv[1], "status")==0) {
        doStatus(argc, argv);
    } else  if (strcmp(argv[1], "diff")==0) {
        doDiff(argc, argv);
    }

    ...
}
```

Every subcommand (pull, commit, etc.) needs to be implemented (doPull, doCommit, etc.) *and* added to the list in the main function, *and* probably to other lists (e. g. some showUsage function). The list of subcommands is scattered and multiplied throughout the application, making it tedious to add or remove commands.

With link-time plug-ins, it's much easier because the implementation of subcommands is perfectly isolated from the code that invokes them:

```cpp
int main(int argc, char** argv) {
    ...

    for (const auto cmd : linktimeplugin::plugins<Command>()) {
        if (strcmp(argv[1], cmd->name())==0) {
            cmd->execute(argc, argv);
        }
    }

    ...
}
```

No list of subcommands needs to be maintained anywhere in the source code. New commands are added simply by implementing their `name()` and `execute()` functions in a class derived from the `Command` base class, hidden in an anonymous namespace somewhere in their own .cpp files. The "show usage" function might look like this:

```cpp
void showUsage(char** argv) {
    std::cout << "Usage: " << argv[0] << " subcommand [ parameter ... ]\n";

    for (const auto cmd : linktimeplugin::plugins<Command>()) {
        std::cout << cmd->usage() << "\n";
    }
}
```

So, when you add a new subcommand, it immediately becomes available on the command line (because `main` picks it up), *and* in the usage message (because `showUsage` picks it up), with no need to modify either of the two.

## How it works

All plug-ins of the same type (=that use the same API) are derived from a common base class. A "registrar object" is created for each such plug-in class that creates an instance of the plug-in class in its constructor. This instance is made available through a public template function, which the application uses to iterate over the plug-ins.

## How to use it

Copy `linktimeplugin.hpp` into an include directory of your choice.

Define a base class for your plug-ins which defines the plug-ins' API as a set of pure virtual functions.

For every plug-in, derive a class from this base class and implement the API functions. The plug-in class is usually defined in its own .cpp file in an anonymous namespace.

Register every plug-in with the `REGISTER_PLUGIN` macro (immediately after the plug-in class definition).

In the application, invoke `linktimeplugin::plugins<Base>()` to retrieve a list of all the plug-ins (where `Base` is the plug-in base class).

You can have more than one plug-in base class per application, each with its own API and its own set of plug-ins.

## Example

Overview:

```cpp
// In all files:
#include <linktimeplugin.hpp>

// In a header file:
class PluginBase : public RegistrarBase<PluginBase> {
public:
    virtual void dosomething() = 0;
};

// In the plug-in cpp file:
namespace {
    class Plugin : public PluginBase {
        void dosomething() override { ... }
    };
    REGISTER_PLUGIN(Plugin);
}

// In the application:
for (const auto plugin : PluginBase::getPlugins()) {
    plugin->dosomething();
}
// OR
std::for_each(
        PluginBase::begin(),
        PluginBase::end(),
        [](PluginBase* x) {
            x->dosomething();
        });
```

Complete example: `main.cpp`. To build and run the example program:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./linktimeplugin
```

---
*Wolfram Rösler • wolfram@roesler-ac.de • https://gitlab.com/wolframroesler • https://twitter.com/wolframroesler • https://www.linkedin.com/in/wolframroesler/*
