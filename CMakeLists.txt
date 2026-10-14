cmake_minimum_required(VERSION 3.16)

# set the project name
project(Greeter LANGUAGES CXX)

# C++ standard used by all the targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pass information to the source code:
configure_file(greeter_config.h.in greeter_config.h)

#add the executable target named HelloWorld
add_executable(greet greet.cpp)

# add binary tree to the list of paths to search includes
target_include_directories(greet PUBLIC "${PROJECT_BINARY_DIR}")