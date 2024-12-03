#include <arba/plug/smart_plugin.hpp>
#include <gtest/gtest.h>

std::filesystem::path plugin_fpath = PLUGIN_PATH;

TEST(SmartPluginTest, TestPluginType_NominalCase_Success)
{
    static_assert(std::is_same_v<plug::smart_plugin,
#ifdef NDEBUG
                                 plug::plugin
#else
                                 plug::safe_plugin
#endif
                                 >);
}

#ifndef NDEBUG
TEST(SmartPluginTest, FindFunctionPtr_BadFunctionType_ExpectException)
{
    try
    {
        plug::smart_plugin plugin(plugin_fpath);
        std::ignore = plugin.find_function_ptr<int (*)()>("generate_str");
        FAIL();
    }
    catch (const std::runtime_error& err)
    {
        std::string err_str(err.what());
        ASSERT_TRUE(err_str.find("Function type of 'generate_str' is not the requested type function")
                    != std::string::npos);
    }
}
#endif
