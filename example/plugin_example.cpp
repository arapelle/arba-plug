#include <arba/plug/plugin.hpp>
#include <arba/plug/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    plug::plugin plugin(PLUGIN_PATH);
    auto generate_int = plugin.find_function_ptr<int (*)()>("generate_int");
    std::cout << generate_int() << std::endl;
    return EXIT_SUCCESS;
}
