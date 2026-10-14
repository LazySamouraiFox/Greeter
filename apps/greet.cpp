#include <greeter/build_info.h>
#include <greeter/message.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if( argc > 1 )
    {
        std::cout << greeter::get_message( argv[1] ) << std::endl;
    }
    else
    {
        std::cout << greeter::get_message() << std::endl;
    }
    
    std::cout << greeter::get_build_info() << std::endl;

    return 0;
}