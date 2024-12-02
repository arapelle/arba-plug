#include <arba/plug/plugin.hpp>
#include <arba/plug/version.hpp>
#include <cstdlib>
#include <iostream>
#include <format>

int main()
{
    std::cout << std::format("{} -> {}", plug::version, plug::dummy()) << std::endl;
    std::cout << "TEST PACKAGE SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
