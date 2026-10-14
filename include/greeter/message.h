#pragma once

#include <string>

namespace greeter
{
    // Return "Hello <name>!".
    // The function only builds the text, it does not print it:
    // this way, it is easy to test.
    std::string get_message(const std::string& name="World");
} // namespace greeter