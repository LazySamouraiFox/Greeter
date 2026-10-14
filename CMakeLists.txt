cmake_minimum_required(VERSION 3.16)

# set the project name
project(Greeter LANGUAGES CXX)

# C++ standard used by all the targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pass information to the source code:
configure_file(cmake/greeter_config.h.in greeter_config.h)

# Libraries (one folder per library in src/)
add_subdirectory(src/greeter)

# Executables
# Options, to change with: cmake -DGREETER_BUILD_APPS=OFF 
option(GREETER_BUILD_APPS "Compile apps" ON)
if(GREETER_BUILD_APPS)
    message(STATUS "Configuring Greeter with executables")
    add_subdirectory(apps)
endif()

# Tests
# Options, to change with: cmake -DGREETER_BUILD_TESTS=OFF
option(GREETER_BUILD_TESTS "Compile tests" ON)
if(GREETER_BUILD_TESTS)
    message(STATUS "Configuring Greeter with tests")
    enable_testing()
    add_subdirectory(tests)
endif()

# ---- Installation: lets other projects use find_package(Greeter) ----
include(CMakePackageConfigHelpers)
# - in the install folder: find_package works after cmake --install
install(EXPORT GreeterTargets FILE GreeterTargets.cmake DESTINATION cmake/)
# GreeterConfig.cmake: the file read by find_package(Greeter)
configure_package_config_file(cmake/GreeterConfig.cmake.in GreeterConfig.cmake INSTALL_DESTINATION cmake/)
install(FILES ${PROJECT_BINARY_DIR}/GreeterConfig.cmake DESTINATION cmake/)

# ---- Packaging: cmake --build build --target package ----
# The archive version is given on the command line, for example:
# cmake -S . -B build -DCPACK_PACKAGE_VERSION=1.2.0
if(WIN32)
 set(CPACK_GENERATOR "ZIP")
else()
 set(CPACK_GENERATOR "TGZ")
endif()
# This must always be last!
include(CPack)