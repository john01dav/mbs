Modular Build System
====================

Overview
--------
Cmake, while a very useful tool, has some flaws. Namely, it requires a very verbose specification of the build
configuration. While this allows for significant control of what is happening, it is often more work to setup for
complex projects. This is where Modular Build System, or MBS, is useful. MBS allows the developer to group their code
into modules. Each module contains cpp and h files, is either a library or executable binary, and can depend on other
modules. When a module depends on another, the headers of the latter module are available to the former, and the former
will be linked with the latter (if the former is a library, then linking the former also links the latter, transitively).
The developer can then run MBS to convert the module specification in YAML to cmake code, that can then be included in
a cmake project. This allows the high-level specification of MBS to be easily used with the low-level details of cmake,
while simultaneously taking advantage of cmake's support for many different build environments.

Building
--------
In MBS, the modules are specified in a mbs.yml file that then needs to be converted to a cmake file to be included in a
cmake project to build. Because, presumably, the users of MBS won't have it installed until they can build it, the cmake
representation is tracked in version control, to allow MBS to be built without already having MBS installed. Other
projects using MBS should not do this, and instead should .gitignore (or otherwise) the cmake output from MBS. Because
MBS includes the cmake file in version control, all that is necessary to build MBS is a recursive clone then a standard
cmake build procedure:

```shell
git clone --recurse-submodules https://github.com/john01dav/mbs.git
mkdir mbs/build
cd mbs/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
```

All of MBS's dependencies are included in the repository via git submodules, or are in the C++17 standard library. A
compiler with C++17 (namely, the filesystem library) support is required to build MBS. The provided cmake files for
building MBS are only tested with GCC and Clang on Linux, however they should work for all other GCC-compatible
compilers. The CMakeLists.txt file may need to be changed to support non-GCC-compatible compilers (ie. MSVC), however 
the code should be compatible with all C++17 compliant compilers.

Installation on Arch Linux
------------
MBS is available in the Arch Linux User Repository (aur) under the name 
[mbs-git](https://aur.archlinux.org/packages/mbs-git/). This AUR package is maintained by a thrid party and unofficial 
with respect to both Arch Linux and MBS, and neither makes any claims regarding its veracity or lack of malicious code.
