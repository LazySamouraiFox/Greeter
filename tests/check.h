#pragma once

#include <iostream>
#include <string>

// Each function returns 0 if the check passes, 1 otherwise (and prints why).
// Usage in a test:
//   int errors = 0;
//   errors += check_equal( my_function(), "expected value" );
//   return errors; // 0 = test passed, anything else = test failed

// Compare a result with the expected value
inline int check_equal( const std::string& result, const std::string& expected )
{
    if( result == expected )
    {
        return 0;
    }
    std::cerr << "FAILED: got \"" << result << "\", expected \"" << expected
              << "\"" << std::endl;
    return 1;
}

// Check that a condition is true
inline int check_true( bool condition, const std::string& message )
{
    if( condition )
    {
        return 0;
    }
    std::cerr << "FAILED: " << message << std::endl;
    return 1;
}
