#include <arba/plug/version.hpp>
#include <gtest/gtest.h>

TEST(project_version_tests, test_version_core)
{
    constexpr unsigned major = 0;
    constexpr unsigned minor = 1;
    constexpr unsigned patch = 0;
    static_assert(arba::plug::version.core() == arba::vrsn::numver(major, minor, patch));
}
