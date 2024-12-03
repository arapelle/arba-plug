#include <gtest/gtest.h>

// class to test
#include <arba/plug/safe_plugin.hpp>
#include <concat_interface/concat_interface.hpp>
#include <format>
#include <iostream>

std::filesystem::path plugin_fpath = PLUGIN_PATH;

// Misc

TEST(PluginImplTest, CheckDefaultFuncNames_Eq_Ok)
{
    ASSERT_EQ(plug::safe_plugin::default_instance_ref_func_name, "instance_ref");
    ASSERT_EQ(plug::safe_plugin::default_instance_cref_func_name, "instance_cref");
    ASSERT_EQ(plug::safe_plugin::default_make_unique_func_name, "make_unique_instance");
    ASSERT_EQ(plug::safe_plugin::default_make_shared_func_name, "make_shared_instance");
}

TEST(PluginBase, PluginFileExtension_NoArg_ExpectNoException)
{
    constexpr std::string_view ext =
#ifdef WIN32
        ".dll";
#else
        ".so";
#endif
    ASSERT_EQ(plug::plugin_file_extension, ext);
}

// Constructors

TEST(SafePluginTest, ConstructorEmpty_NominalCase_ExpectNoException)
{
    try
    {
        plug::safe_plugin plugin;
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(SafePluginTest, Constructor_ExistingLibrary_ExpectNoException)
{
    try
    {
        plug::safe_plugin plugin(plugin_fpath);
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(SafePluginTest, Constructor_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "concat/libunfound";

    try
    {
        plug::safe_plugin plugin(std::filesystem::current_path() / "concat/libunfound");
        FAIL();
    }
    catch (const plug::plugin_load_error& exception)
    {
        std::string expected_msg(std::format("Exception occurred while loading plugin: {}", lib_path.generic_string()));
        std::string err_msg(exception.what());
        ASSERT_EQ(err_msg.find(expected_msg), 0);
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// LoadFromFile

TEST(SafePluginTest, LoadFromFile_ExistingLibraryWithExtension_ExpectNoException)
{
    try
    {
        plug::safe_plugin plugin;
        plugin.load_from_file(plugin_fpath.generic_string() + std::string(plug::plugin_file_extension));
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(SafePluginTest, LoadFromFile_ExistingLibraryNoExtension_ExpectNoException)
{
    try
    {
        plug::safe_plugin plugin;
        plugin.load_from_file(plugin_fpath);
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(SafePluginTest, LoadFromFile_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "concat/libunfound";
    try
    {
        plug::safe_plugin plugin;
        plugin.load_from_file(lib_path);
        FAIL();
    }
    catch (const plug::plugin_load_error& exception)
    {
        constexpr std::string_view expected_msg_fmt = "Exception occurred while loading plugin: {}";
        std::string expected_msg(std::format(expected_msg_fmt, lib_path.generic_string()));
        ASSERT_EQ(std::string(exception.what()).find(expected_msg), 0);
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// Unload

TEST(SafePluginTest, Unload_NomicalCase_ExpectNoException)
{
    try
    {
        plug::safe_plugin plugin(plugin_fpath);
        ASSERT_TRUE(plugin.is_loaded());
        plugin.unload();
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// FindFunctionPtr

TEST(SafePluginTest, FindFunctionPtr_FunctionName_ReturnNotNullFunctionPtr)
{
    std::string res;
    plug::safe_plugin plugin(plugin_fpath);
    auto execute = plugin.find_function_ptr<void (*)(std::string&, std::string_view, const std::string&)>("execute");
    ASSERT_NE(execute, nullptr);
    execute(res, "a", "b");
    ASSERT_EQ(res, "a-b");
}

TEST(SafePluginTest, FindFunctionPtr_BadFunctionType_ExpectException)
{
    try
    {
        plug::safe_plugin plugin(plugin_fpath);
        std::ignore = plugin.find_function_ptr<void (*)(float&)>("execute");
        FAIL();
    }
    catch (const std::runtime_error& err)
    {
        std::string err_str(err.what());
        ASSERT_TRUE(err_str.find("Function type of 'execute' is not the requested type function") != std::string::npos);
    }
}

TEST(SafePluginTest, FindFunctionPtr_UnregisteredFunction_ExpectException)
{
    try
    {
        plug::safe_plugin plugin(plugin_fpath);
        std::ignore = plugin.find_function_ptr<int (*)(std::string_view)>("unregistered_function");
        FAIL();
    }
    catch (const std::runtime_error& err)
    {
        std::string err_str(err.what());
        ASSERT_TRUE(err_str.find("Function 'unregistered_function' exists in plugin, but its type cannot be checked. "
                                 "Did you forget to use ARBA_PLUG_REGISTER_PLUGIN_FUNCTION() ?")
                    != std::string::npos);
    }
}

TEST(SafePluginTest, FindFunctionPtr_FunctionName_ExpectException)
{
    std::string_view function_name("notFoundFunction");

    try
    {
        plug::safe_plugin plugin(plugin_fpath);
        plugin.find_function_ptr<void (*)(int&)>(function_name);
    }
    catch (const plug::plugin_find_symbol_error& exception)
    {
        std::string msg(exception.what());
        ASSERT_EQ(msg.find("Exception occurred while looking for address of"), 0);
        ASSERT_NE(msg.find(function_name), std::string::npos);
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// MakeUniqueInstance

TEST(SafePluginTest, MakeUniqueInstance_FunctionExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance");

    plug::safe_plugin plugin(plugin_fpath);
    std::unique_ptr<ConcatInterface> instance = plugin.make_unique_instance<ConcatInterface>(function_name);
    ASSERT_EQ(instance->concat("a", "b"), "a-b");
}

TEST(SafePluginTest, MakeUniqueInstance_FunctionTakingArgsExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance_from_args");

    plug::safe_plugin plugin(plugin_fpath);
    std::unique_ptr<ConcatInterface> instance;

    std::string a = "(";
    std::string b = "(";
    std::string z = "))";

    ASSERT_EQ(b, "(");
    instance = plugin.make_unique_instance<ConcatInterface, std::string_view, std::string&, const std::string&>(
        function_name, a, b, z);
    std::string str = instance->concat("aa", "bb");
    ASSERT_EQ(str, "((aa-bb))");
    ASSERT_EQ(b, "((");
}

// MakeSharedInstance

TEST(SafePluginTest, MakeSharedInstance_FunctionExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance");

    plug::safe_plugin plugin(plugin_fpath);
    std::shared_ptr<ConcatInterface> instance = plugin.make_shared_instance<ConcatInterface>(function_name);
    ASSERT_EQ(instance->concat("a", "b"), "a-b");
}

TEST(SafePluginTest, MakeSharedInstance_FunctionTakingArgsExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance_from_args");

    std::string a = "(";
    std::string b = "(";
    std::string z = "))";

    plug::safe_plugin plugin(plugin_fpath);
    ASSERT_EQ(b, "(");
    std::shared_ptr<ConcatInterface> instance =
        plugin.make_shared_instance<ConcatInterface, std::string_view, std::string&, const std::string&>(function_name,
                                                                                                         a, b, z);
    std::string str = instance->concat("aa", "bb");
    ASSERT_EQ(str, "((aa-bb))");
    ASSERT_EQ(b, "((");
}

// InstanceRef & InstanceCref

TEST(SafePluginTest, InstanceRef_FunctionExists_ReturnTypeRef)
{
    std::string_view function_name("default_concat");

    plug::safe_plugin plugin(plugin_fpath);
    ConcatInterface& instance = plugin.instance_ref<ConcatInterface>(function_name);
    ASSERT_EQ(instance.concat("a", "b"), "a-b");
}

TEST(SafePluginTest, InstanceCref_FunctionExists_ReturnTypeConstRef)
{
    std::string_view function_name("default_const_concat");

    plug::safe_plugin plugin(plugin_fpath);
    const ConcatInterface& instance = plugin.instance_cref<ConcatInterface>(function_name);
    ASSERT_EQ(instance.concat("a", "b"), "a-b");
}

// Move Constructor

TEST(SafePluginTest, MoveConstructor_ExistingLibrary_ExpectNoException)
{
    try
    {
        std::unique_ptr plugin_uptr = std::make_unique<plug::safe_plugin>(plugin_fpath);
        ASSERT_TRUE(plugin_uptr->is_loaded());
        plug::safe_plugin other_plugin(std::move(*plugin_uptr));
        ASSERT_TRUE(other_plugin.is_loaded());
        ASSERT_FALSE(plugin_uptr->is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// Move Assignment

TEST(SafePluginTest, MoveAssignment_ExistingLibrary_ExpectNoException)
{
    try
    {
        std::unique_ptr plugin_uptr = std::make_unique<plug::safe_plugin>(plugin_fpath);
        ASSERT_TRUE(plugin_uptr->is_loaded());
        plug::safe_plugin other_plugin;
        other_plugin = std::move(*plugin_uptr);
        ASSERT_TRUE(other_plugin.is_loaded());
        ASSERT_FALSE(plugin_uptr->is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}
