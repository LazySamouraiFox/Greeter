#include "check.h"

#include <greeter/build_info.h>

int main()
{
    int errors = 0;

    // The system must be one of the systems we build on
    const std::string system = greeter::get_build_system();
    errors += check_true(
        system == "Linux" || system == "Windows" || system == "Darwin",
        "unknown build system \"" + system + "\"" );

    // The text must start with "built on <system>"
    const std::string start = "built on " + system;
    errors += check_equal(
        greeter::get_build_info().substr( 0, start.size() ), start );

    return errors;
}
