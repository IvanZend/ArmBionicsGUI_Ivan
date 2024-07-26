# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.26.1)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(qwt-populate NONE)


# Pass through things we've already detected in the main project to avoid
# paying the cost of redetecting them again in ExternalProject_Add()
set(GIT_EXECUTABLE [==[/usr/local/bin/git]==])
set(GIT_VERSION_STRING [==[2.16.3]==])
set_property(GLOBAL PROPERTY _CMAKE_FindGit_GIT_EXECUTABLE_VERSION
  [==[/usr/local/bin/git;2.16.3]==]
)


include(ExternalProject)
ExternalProject_Add(qwt-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://git.code.sf.net/p/qwt/git" "GIT_TAG" "878e90e5fe02f8e752d5d5eefea629fb4fbe54c0" "PATCH_COMMAND" "git" "apply" "/Users/aramsevagavarian/ArmBionics/qwt-6.2.1.patch" "--unsafe-paths"
                    SOURCE_DIR          "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-src"
                    BINARY_DIR          "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


