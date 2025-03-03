#include <gtest/gtest.h>

// class to test
#include <arba/plug/plugin.hpp>

#include <concat_interface/concat_interface.hpp>

#include <format>

std::filesystem::path plugin_fpath = PLUGIN_PATH;

// Constructors

TEST(PluginTest, ConstructorEmpty_NominalCase_ExpectNoException)
{
    try
    {
        plug::plugin plugin;
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(PluginTest, Constructor_ExistingLibrary_ExpectNoException)
{
    try
    {
        plug::plugin plugin(plugin_fpath);
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(PluginTest, Constructor_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "concat/libunfound";

    try
    {
        plug::plugin plugin(std::filesystem::current_path() / "concat/libunfound");
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

TEST(PluginTest, LoadFromFile_ExistingLibraryWithExtension_ExpectNoException)
{
    try
    {
        plug::plugin plugin;
        plugin.load_from_file(plugin_fpath.generic_string() + std::string(plug::plugin_file_extension));
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(PluginTest, LoadFromFile_ExistingLibraryNoExtension_ExpectNoException)
{
    try
    {
        plug::plugin plugin;
        plugin.load_from_file(plugin_fpath);
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(PluginTest, LoadFromFile_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "concat/libunfound";
    try
    {
        plug::plugin plugin;
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

TEST(PluginTest, Unload_NomicalCase_ExpectNoException)
{
    try
    {
        plug::plugin plugin(plugin_fpath);
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

TEST(PluginTest, FindFunctionPtr_FunctionName_ReturnNotNullFunctionPtr)
{
    std::string res;
    plug::plugin plugin(plugin_fpath);
    auto execute = plugin.find_function_ptr<void (*)(std::string&, std::string_view, const std::string&)>("execute");
    ASSERT_NE(execute, nullptr);
    execute(res, "a", "b");
    ASSERT_EQ(res, "a-b");
}

TEST(PluginTest, FindFunctionPtr_FunctionName_ExpectException)
{
    std::string_view function_name("notFoundFunction");

    try
    {
        plug::plugin plugin(plugin_fpath);
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

TEST(PluginTest, MakeUniqueInstance_FunctionExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance");

    plug::plugin plugin(plugin_fpath);
    std::unique_ptr<ConcatInterface> instance = plugin.make_unique_instance<ConcatInterface>(function_name);
    ASSERT_EQ(instance->concat("a", "b"), "a-b");
}

TEST(PluginTest, MakeUniqueInstance_FunctionTakingArgsExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance_from_args");

    plug::plugin plugin(plugin_fpath);
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

TEST(PluginTest, MakeSharedInstance_FunctionExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance");

    plug::plugin plugin(plugin_fpath);
    std::shared_ptr<ConcatInterface> instance = plugin.make_shared_instance<ConcatInterface>(function_name);
    ASSERT_EQ(instance->concat("a", "b"), "a-b");
}

TEST(PluginTest, MakeSharedInstance_FunctionTakingArgsExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance_from_args");

    std::string a = "(";
    std::string b = "(";
    std::string z = "))";

    plug::plugin plugin(plugin_fpath);
    ASSERT_EQ(b, "(");
    std::shared_ptr<ConcatInterface> instance =
        plugin.make_shared_instance<ConcatInterface, std::string_view, std::string&, const std::string&>(function_name,
                                                                                                         a, b, z);
    std::string str = instance->concat("aa", "bb");
    ASSERT_EQ(str, "((aa-bb))");
    ASSERT_EQ(b, "((");
}

// InstanceRef & InstanceCref

TEST(PluginTest, InstanceRef_FunctionExists_ReturnTypeRef)
{
    std::string_view function_name("default_concat");

    plug::plugin plugin(plugin_fpath);
    ConcatInterface& instance = plugin.instance_ref<ConcatInterface>(function_name);
    ASSERT_EQ(instance.concat("a", "b"), "a-b");
}

TEST(PluginTest, InstanceCref_FunctionExists_ReturnTypeConstRef)
{
    std::string_view function_name("default_const_concat");

    plug::plugin plugin(plugin_fpath);
    const ConcatInterface& instance = plugin.instance_cref<ConcatInterface>(function_name);
    ASSERT_EQ(instance.concat("a", "b"), "a-b");
}

// Move Constructor

TEST(PluginTest, MoveConstructor_ExistingLibrary_ExpectNoException)
{
    try
    {
        std::unique_ptr plugin_uptr = std::make_unique<plug::plugin>(plugin_fpath);
        ASSERT_TRUE(plugin_uptr->is_loaded());
        plug::plugin other_plugin(std::move(*plugin_uptr));
        ASSERT_TRUE(other_plugin.is_loaded());
        ASSERT_FALSE(plugin_uptr->is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// Move Assignment

TEST(PluginTest, MoveAssignment_ExistingLibrary_ExpectNoException)
{
    try
    {
        std::unique_ptr plugin_uptr = std::make_unique<plug::plugin>(plugin_fpath);
        ASSERT_TRUE(plugin_uptr->is_loaded());
        plug::plugin other_plugin;
        other_plugin = std::move(*plugin_uptr);
        ASSERT_TRUE(other_plugin.is_loaded());
        ASSERT_FALSE(plugin_uptr->is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}
