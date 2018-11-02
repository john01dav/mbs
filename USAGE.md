Modular Build System Usage Tutorial
===================================

Overview
--------
An MBS project is made up of modules, or conceptual groups of code containing C and C++ source files and headers. Each
module is either a library or executable binary. A library can be shared, static, or both. Each module also has
zero or more dependencies, which will be linked to the module. If an application module depends on a library module, and
that library module depends on another library module, then the application module is able to use the second library
module and will be linked to it.

Directory Structure
-------------------
A typical MBS project's directory structure looks similar to the following:

    example-project
    ├── build
    |   ├ …
    ├── CMakeLists.txt
    ├── mbs.yml
    └── modules
        ├── module1
        │   ├── include
        │   │   ├── a.h
        │   │   └── b.h
        │   └── source
        │       ├── a.cpp
        │       └── b.cpp
        └── module2
            ├── include
            │   └── c.h
            └── source
                ├── c.cpp
                └── main.cpp

As can be seen above, each module exists in the modules directory, and each module has an include and source folder. In
addition to the modules folder, there is an mbs.yml file that specifies information about the project that can't be 
represented purely in the filesystem, such as the type of each module and  what depends on what. The build folder, with 
its contents not shown, contains cmake's project files. The CMakeLists.txt contains the cmake code that is not generated
by MBS, and is instead supplied by the user.

mbs.yml File
------------
As was stated previously, each MBS project contains a single mbs.yml file. This file, as the name suggests, is in YAML
format. It contains the information that MBS needs to build a cmake project specification. The mbs.yml for building MBS
itself is the following:

    mbs-format-version: 0
    modules:
      libmbs:
        type: library
        library-type: static
      mbs:
        type: application
        dependencies:
         - libmbs-static

Although the order of the nodes in mbs.yml is irrelevant, the order above is used for convenience while explaining. The
first setting in this file is `mbs-format-version`. This property specifies the minimum MBS format version necessary to
parse the file. The format version is different from the MBS version, and starts at 0 and increments by one with each
format change. New versions of MBS may or may not increment the value, but no single new version will ever increment it
by more than one. MBS will refuse to process the mbs.yml file if the version is greater than the version supported by
the running MBS binary.

Net, there is the modules node. In the modules node, there is a sub-node for each module. The name of each sub-node is
the name of the module it represents. Each module must specify, at minimum, it's type. The two valid options for type
are `application` and `library`. If the `application` type is selected, no further properties are required. If the
`library` type is selected, then the `library-type` property must be set to either `static`, `dynamic`, or `both`. The
`dependencies` property is a list, and optionally provides a list of library modules upon which the module it is located
in depends. Each item in this list takes the form `<module-name>-static` or `<module-name>-dynamic`. It is undefined
behaviour to list a module that does not exist, or to ask for a library type that does not exist for the specified
module. Specifying a library here will make that library's includes available to the module it is specified in, and, if
the module it is specified in is an application, link to that module, or if it is a library then link to any application
using that library. This property can *only* be used with *MBS modules that are libraries* it will *not* work with
system libraries, CMake libraries defined outside of the mbs.yml file, or anything else that isn't a module of type
library in the same mbs.yml file. It is considered undefined behavior to use it in any other way.

Using MBS With Cmake
--------------------
To use MBS at all, and especially to unlock its most powerful features, it must be used with cmake. To facilitate this,
run the `mbs <mbs.yml> <mbs.cmake>` to generate an mbs.cmake file. The mbs.cmake file contains cmake commands that will
create cmake targets for each module in the mbs.yml. Those targets will consist of the correct source files, include the
correct directories for any module dependencies, and modules that depend on other modules to the latter modules. It will
*not*, however, do anything for libraries specified outside of MBS. For example, to use SFML, you must use CMake's
standard find_package tool, or some other Cmake-compatible library linking tool. To facilitate this, mbs will create one
or more cmake targets for each module. The exact targets created depend on the type of the module. An application module
will create exactly one target of the form `module-<module name>`. For example, the `mbs` module in MBS's own mbs.yml
creates the cmake target `module-mbs`. A library type module will create either one or two targets of the form
`module-<module name>-<static/dynamic>`. For example, the `libmbs` module in MBS's mbs.yml will create the
cmake .target `module-libmbs-static`. To link to system libraries, one can then call target_link_libraries, and other
target-specific or unspecific commands as normal. For example, to link to SFML in a hypothetical project, a
CMakeLists.txt similar to the following can be used:

    cmake_minimum_required(VERSION 3.5)
    project(examplesfmlproject)
    
    set(CMAKE_CXX_STANDARD 11)
    
    include(mbs.cmake)
    
    find_package(SFML REQUIRED COMPONENTS window)
    target_link_libraries(module-main sfml-window)