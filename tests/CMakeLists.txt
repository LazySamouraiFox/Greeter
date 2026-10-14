cmake_minimum_required(VERSION 3.16)

# add_greeter_test(<name>): compile <name>.cpp, link it with greeter
# and register it in ctest. A test passes if main() returns 0.
function(add_greeter_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE greeter)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

# One line per test
add_greeter_test(test-message)
add_greeter_test(test-build-info)
