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