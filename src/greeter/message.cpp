#include <greeter/message.h>


namespace greeter
{
    std::string get_message(const std::string& name)
    {
        return "Hello " + name + " !";
    }
} // namespace greeter