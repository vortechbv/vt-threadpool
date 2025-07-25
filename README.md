vt-threadpool
=============

[![Build Status](https://github.com/vortechbv/vt-threadpool/actions/workflows/ci.yml/badge.svg)](https://github.com/vortechbv/vt-threadpool/actions/workflows/ci.yml)

Simple thread pool library for C++. The primary functionality of this library, the member-function `vt::thread_pool::run(...)`, is similar to `std::async(std::launch::async, ...)`, with the added benefit that you have more control over the size, scope and state of the thread pool.

The library can be used as an alternative to OpenMP. While this library will usually have a bit more runtime overhead compared to OpenMP, it does properly handle and propagate C++ exceptions that occur in the threads.

Getting started
---------------

The library uses C++17 features and assumes a C++17 compliant compiler. Compatibility with the following compilers is tested:

- GCC 10.5
- Clang 13.0
- MSVC vs2022

If you're using CMake, you can include this library in your project using `FetchContent` as follows:

```cmake
include(FetchContent)

FetchContent_Declare(
    vt-threadpool
    GIT_REPOSITORY https://github.com/vortechbv/vt-threadpool.git
    GIT_TAG v0.1.0
)
set(VT_ENABLE_TESTING OFF)
set(VT_ENABLE_INSTALL OFF)
FetchContent_MakeAvailable(vt-threadpool)

# ...

target_link_libraries(<your-target> vt::threadpool)
```

The library internals can use more efficient standard library components if C++20 or C++23 is available. The C++ standard used can be configured through the `CMAKE_CXX_STANDARD` or `VT_CXX_STD` flag. The default is `17`.

A reference section containing documentation of the API and usage examples can be found [here](doc/vt/threadpool/readme.md#top).

For maintainers
---------------

To build the library and accompanying test suite, you will need CMake 3.15 or newer. Instructions follow below.

### Building

First, you will need to use CMake to generate build files for your platform:

mkdir <build-dir>
cd <build-dir>
cmake <vt-threadpool-root-dir>

Here `<build-dir>` can be any directory. It is advised to pick a directory outside of the project's source tree, e.g. to avoid the build files from showing up when `grep`-ing the source tree. The `<vt-threadpool-root-dir>` is the directory that contains the `CMakeLists.txt`-file.

After generating the build files, you can build the test suite using the build files' native method. For example, if Unix Makefiles were generated, you can run `make`, or if a Visual Studio solution was generated, you can use `msbuild` or the Visual Studio IDE.

The install script is executed when building the `install`-target.

### Testing

You can run the test suite by executing `vt-threadpool-test`/`vt-threadpool-test.exe`. Run with `--help` for more options. Note that Visual Studio places the output binaries in a `Debug`/`Release` subdirectory of your build directory.

### Options

Common CMake options for the build are listed below. See the CMake documentation for more options.

#### Build Type

Some build systems (such as Visual Studio) have multiple build types. For build systems with only one build type (such as Makefiles) the default build type will be `Release`. To specify a different build type, add the `-DCMAKE_BUILD_TYPE=<build-type>` flag when executing CMake.

The available build types are: `Debug`, `Release`, `RelWithDebInfo` and `MinSizeRel`.

#### Optional compilation

Compilation of the test suite is not mandatory. You can disable it by adding the `-DVT_ENABLE_TESTING=OFF` flag when executing CMake.

#### External Libraries

For testing purposes, vt-threadpool uses the Catch testing framework. By default, Catch will be downloaded from GitHub, unless you specified to disable the compilation of the test suite. If the download is not possible, you can specify an alternative download location.

To specify an alternative download location for Catch, add the `-VT_CATCH_GIT_REPOSITORY=<url>` flag when executing CMake. Here `<url>` should lead to the Catch Git repository.
