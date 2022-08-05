cmake_minimum_required(VERSION 3.1)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

include(FindPkgConfig)
find_package(Qt5Core 5.9 REQUIRED)
find_package(Qt5DBus 5.9 REQUIRED)

# Disable all deprecated qt functions in and before qt version 5.9
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050900)

# Use the fast string builder
add_definitions(-DQT_USE_QSTRINGBUILDER)

# Standard install paths
include(GNUInstallDirs)

# Prefer not to use Qt's keywords
add_definitions(-DQT_NO_KEYWORDS)

# add subdirectories to build
add_subdirectory(src)
add_subdirectory(aux)
