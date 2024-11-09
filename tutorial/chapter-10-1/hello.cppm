// --8<-- [start:snippet0]
module;

#include <iostream>
#include <string_view>

export module hello;

export namespace Hello {

void printHello(std::string_view name)
{
    std::cout << "Hello, " << name << '!' << std::endl;
}

} // namespace Hello
// --8<-- [end:snippet0]
