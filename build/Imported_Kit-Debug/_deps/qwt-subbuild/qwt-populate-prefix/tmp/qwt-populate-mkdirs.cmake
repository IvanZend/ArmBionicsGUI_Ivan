# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-src"
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-build"
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix"
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix/tmp"
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix/src/qwt-populate-stamp"
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix/src"
  "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix/src/qwt-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix/src/qwt-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/aramsevagavarian/ArmBionics/build/Imported_Kit-Debug/_deps/qwt-subbuild/qwt-populate-prefix/src/qwt-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
