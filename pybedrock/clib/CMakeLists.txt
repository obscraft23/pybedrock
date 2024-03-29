# Copyright (c) 2023 obscraft23 obscraft23@gmail.com
# The code is distributed under the MIT license.
# https://github.com/obscraft23/pybedrock/blob/main/LICENSE
#
# This code is a modification of a code in mcberepair.
# https://github.com/reedacartwright/mcberepair 
# 
# The original license is as follows:
# Copyright (c) 2019-2020 Reed A. Cartwright <reed@cartwright.ht>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds not allowed. Please run CMake from the './build' directory. You may need to remove CMakeCache.txt.")
endif()

#set(CMAKE_C_COMPILER "gcc")
#set(CMAKE_CXX_COMPILER "g++")
################################################################################
#  Project Configuration

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
#set(CMAKE_BUILD_TYPE_INIT "RelWithDebInfo")
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)

################################################################################
# Init Project

project(pybedrock
  DESCRIPTION "Minecraft Bedrock Utilities for Python"
  HOMEPAGE_URL "https://github.com/obscraft23/pybedrock"
  LANGUAGES CXX
)

option(LEVELDB_BUILD_TESTS OFF)
option(LEVELDB_INSTALL OFF)
add_subdirectory(leveldb-mcpe)

# GOOGLE disables RTTI for leveldb 
# BUT ENABLE RTTI by obscraft23
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Enable RTTI.
  string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR /MD /d2FH4-")
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  #add_compile_options("-Wl,-rpath,")
else()
  # Enable RTTI.
  string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fpic")
  set(CMAKE_C_COMPILER "gcc")
  set(CMAKE_CXX_COMPILER "g++")
  #set (CMAKE_SHARED_LINKER_FLAGS "-Wl,-no_weak_imports")
  #link_libraries("-Wl,-no_weak_imports")
  #add_compile_options("-Werror=partial-availability")
endif()

################################################################################
# Installation Information

string(TOLOWER ${CMAKE_PROJECT_NAME} basename_for_install)

set(CMAKE_DATA_DIR "share/${basename_for_install}" CACHE STRING
  "Install location for data (relative to prefix).")
set(CMAKE_DOC_DIR "doc/${basename_for_install}" CACHE STRING
  "Install location for documentation (relative to prefix).")
set(CMAKE_MAN_DIR "man" CACHE STRING
  "Install location for man pages (relative to prefix).")
mark_as_advanced(CMAKE_DATA_DIR CMAKE_DOC_DIR CMAKE_MAN_DIR)

#nstall(FILES README.md LICENSE DESTINATION ${CMAKE_DOC_DIR})

################################################################################
# Building binary

if( ${CMAKE_VERSION} LESS 3.12 )
    find_package(PythonLibs REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    
else()
    find_package(Python COMPONENTS Interpreter Development)
    include_directories(${Python_INCLUDE_DIRS})
endif()

add_library(leveldbhandler SHARED
  leveldbhandler.cpp
  listkeys.cpp
  dumpkey.cpp
  writekey.cpp
  rmkeys.cpp
  readNBT.cpp
  readNBT_big.cpp
  writeNBT.cpp
  subchunk.cpp
  db.hpp
  mcbekey.hpp
  perenc.hpp
  slurp.hpp
)
target_link_libraries(leveldbhandler leveldb)
target_include_directories(leveldbhandler PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(leveldbhandler PUBLIC ${PYTHON_INCLUDE_DIRS})
target_link_libraries(leveldbhandler ${PYTHON_LIBRARIES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set_target_properties(leveldbhandler PROPERTIES SUFFIX ".pyd")
elseif($ENV{PYTHON_MAC_LIBRARIES_ON} STREQUAL "on")
  set(PYTHON_MAC_LIBRARIES $ENV{PYTHON_MAC_LIBRARIES_ENV})
  target_link_libraries(leveldbhandler ${PYTHON_MAC_LIBRARIES})
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  target_compile_options(leveldbhandler PUBLIC /MD
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>
      $<$<CXX_COMPILER_ID:MSVC>:/W4>)
else()
  target_compile_options(leveldbhandler PUBLIC -fpic
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endif()

install(TARGETS leveldbhandler RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR})

# copy any dlls installed by vcpkg on windows
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  install(DIRECTORY C:/vcpkg/installed/x64-windows/bin/ TYPE LIB FILES_MATCHING PATTERN "*.dll")
endif()