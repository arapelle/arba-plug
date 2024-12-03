#pragma once

#include "plugin_base.hpp"

#include <filesystem>
#include <memory>
#include <type_traits>

inline namespace arba
{
namespace plug
{

/**
 * @brief The plugin_impl class
 */
template <class PluginType>
class plugin_impl : public plugin_base
{
protected:
    inline plugin_impl() {}

    explicit plugin_impl(const std::filesystem::path& plugin_path) : plugin_base(plugin_path) {}

public:
    plugin_impl(plugin_impl&&) = default;
    plugin_impl& operator=(plugin_impl&&) = default;

    static constexpr std::string_view default_instance_ref_func_name = "instance_ref";

    /**
     * @brief instance_ref Find a function returning a reference to a global variable and call it to return the
     * reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin.
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    InstanceType& instance_ref(const std::string_view getter_function_name = default_instance_ref_func_name)
    {
        using MainObjectGetter = InstanceType& (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        MainObjectGetter getter = self.template find_function_ptr<MainObjectGetter>(getter_function_name);
        return getter();
    }

    static constexpr std::string_view default_instance_cref_func_name = "instance_cref";

    /**
     * @brief instance_cref Find a function returning a const reference to a global variable and call it to return the
     * const reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin.
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be const InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    const InstanceType& instance_cref(const std::string_view getter_function_name = default_instance_cref_func_name)
    {
        using MainObjectMaker = const InstanceType& (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        MainObjectMaker getter = self.template find_function_ptr<MainObjectMaker>(getter_function_name);
        return getter();
    }

    static constexpr std::string_view default_make_unique_func_name = "make_unique_instance";

    /**
     * @brief make_unique_instance Find a function making a new instance stored in a std::unique_ptr and call it to
     * return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_instance")
     * @return A std::unique_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType>
        requires std::has_virtual_destructor_v<ClassType>
    std::unique_ptr<ClassType>
    make_unique_instance(const std::string_view maker_function_name = default_make_unique_func_name)
    {
        using InstanceMaker = std::unique_ptr<ClassType> (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

    /**
     * @brief make_unique_instance Find a function making a new instance stored in a std::unique_ptr and call it to
     * return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam ArgsT... The types of the arguments to pass to the maker function.
     * @param maker_function_name The name of the maker function to find in the plugin.
     * @param args The arguments to pass to the maker function.
     * @return A std::unique_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     * @warning All args types must be explicitly provided. (make_unique_instance<InstanceType, Parameter1Type>(...))
     */
    template <typename ClassType, typename... ArgsT>
        requires std::has_virtual_destructor_v<ClassType> && (sizeof...(ArgsT) > 0)
    std::unique_ptr<ClassType> make_unique_instance(const std::string_view maker_function_name, ArgsT... args)
    {
        using InstanceMaker = std::unique_ptr<ClassType> (*)(ArgsT...);
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker(args...);
    }

    static constexpr std::string_view default_make_shared_func_name = "make_shared_instance";

    /**
     * @brief make_shared_instance Find a function making a new instance stored in a std::shared_ptr and call it to
     * return the std::shared_ptr.
     * @tparam ClassType The type of the made instance.
     * @param maker_function_name The name of the maker function to find in the plugin.
     * @return A std::shared_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::shared_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType>
        requires std::has_virtual_destructor_v<ClassType>
    std::shared_ptr<ClassType>
    make_shared_instance(const std::string_view maker_function_name = default_make_shared_func_name)
    {
        using InstanceMaker = std::shared_ptr<ClassType> (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

    /**
     * @brief make_shared_instance Find a function making a new instance stored in a std::shared_ptr and call it to
     * return the std::shared_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam ArgsT... The types of the arguments to pass to the maker function.
     * @param maker_function_name The name of the maker function to find in the plugin.
     * @param args The arguments to pass to the maker function.
     * @return A std::shared_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::shared_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     * @warning All args types must be explicitly provided. (make_shared_instance<InstanceType, Parameter1Type>(...))
     */
    template <typename ClassType, typename... ArgsT>
        requires std::has_virtual_destructor_v<ClassType> && (sizeof...(ArgsT) > 0)
    std::shared_ptr<ClassType> make_shared_instance(const std::string_view maker_function_name, ArgsT... args)
    {
        using InstanceMaker = std::shared_ptr<ClassType> (*)(ArgsT...);
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker(args...);
    }
};

} // namespace plug
} // namespace arba
