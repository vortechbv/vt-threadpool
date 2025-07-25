# Copyright (c) 2017-2025 VORtech b.v.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(vt-threadpool CXX)

include(FetchContent)
include(GNUInstallDirs)


# User options
##############

option(VT_ENABLE_TESTING "Build the test-suite" ON)
option(VT_ENABLE_INSTALL "Enable installation of header files and libraries" ON)
option(VT_POSITION_INDEPENDENT_CODE "PIC option for library" ON)

if(CMAKE_CXX_STANDARD AND "${CMAKE_CXX_STANDARD}" GREATER 17)
    set(VT_CXX_STD_DEFAULT ${CMAKE_CXX_STANDARD})
else()
    set(VT_CXX_STD_DEFAULT 17)
endif()

set(
    VT_CXX_STD
    ${VT_CXX_STD_DEFAULT}
    CACHE
    STRING
    "C++ standard this library should use."
)

set(
    VT_CATCH_GIT_REPOSITORY
    https://github.com/catchorg/Catch2.git
    CACHE
    STRING
    "URL to Catch git repository. Can be a local clone."
)


# Build type configuration
##########################

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type specified, defaulting to Release.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()


# Third-party dependencies
##########################

find_package(Threads REQUIRED)

if(VT_ENABLE_TESTING)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY ${VT_CATCH_GIT_REPOSITORY}
        GIT_TAG v3.8.1
    )

    FetchContent_MakeAvailable(Catch2)
endif()


# Compile options
#################

set(
    VT_THREADPOOL_GNU_COMPILE_OPTIONS
    -pedantic-errors
    -Wall
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wfloat-equal
    -Wlogical-op
    -Wmissing-include-dirs
    -Wold-style-cast
    -Woverloaded-virtual
    -Wsign-conversion
    -Wsuggest-override
    -Wundef
    -Wuseless-cast
)

set(
    VT_THREADPOOL_Clang_COMPILE_OPTIONS
    -pedantic-errors
    -Weverything
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-missing-braces
    -Wno-padded
)

set(
    VT_THREADPOOL_MSVC_COMPILE_OPTIONS
    /W4
    /volatile:iso
    /Zc:strictStrings
    /Zc:rvalueCast
)


# Library target
################

add_library(
    vt-threadpool STATIC
    "${CMAKE_CURRENT_LIST_DIR}/src/vt/thread_pool.cpp"
)

target_include_directories(
    vt-threadpool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)
target_link_libraries(vt-threadpool Threads::Threads)
target_compile_options(
    vt-threadpool
    PRIVATE ${VT_THREADPOOL_${CMAKE_CXX_COMPILER_ID}_COMPILE_OPTIONS}
)
set_target_properties(
    vt-threadpool
    PROPERTIES
        POSITION_INDEPENDENT_CODE ${VT_POSITION_INDEPENDENT_CODE}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN OFF
        CXX_EXTENSIONS OFF
        EXPORT_NAME threadpool
)
target_compile_features(vt-threadpool PUBLIC cxx_std_${VT_CXX_STD})

add_library(vt::threadpool ALIAS vt-threadpool)


# Test target
#############

if(VT_ENABLE_TESTING)
    add_executable(
        vt-threadpool-test
        "${CMAKE_CURRENT_LIST_DIR}/test/vt/channel_test.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/test/vt/thread_pool_test.cpp"
    )
    target_link_libraries(
        vt-threadpool-test
        vt-threadpool
        Catch2::Catch2WithMain
    )
    target_compile_options(
        vt-threadpool-test
        PRIVATE ${VT_THREADPOOL_${CMAKE_CXX_COMPILER_ID}_COMPILE_OPTIONS}
    )
endif()


# Install target
################

if(VT_ENABLE_INSTALL)
    install(TARGETS vt-threadpool EXPORT vt-threadpool-targets)
    install(
        EXPORT vt-threadpool-targets
        NAMESPACE vt::
        DESTINATION "${CMAKE_INSTALL_DATADIR}/vt/threadpool/cmake"
    )
    install(
        DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/vt"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endif()
