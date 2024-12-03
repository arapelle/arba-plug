#pragma once

#include "plugin_impl.hpp"

#include <memory>
#include <type_traits>

inline namespace arba
{
namespace plug
{

/**
 * @brief The plugin class
 */
class plugin : public plugin_impl<plugin>
{
private:
    using base_ = plugin_impl<plugin>;

public:
    inline plugin() {}

    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     */
    explicit plugin(const std::filesystem::path& plugin_path) : base_(plugin_path) {}

    plugin(plugin&&) = default;
    plugin& operator=(plugin&&) = default;

    /**
     * @brief find_function_ptr Find the address of the function with a given name.
     * @tparam PointerType Signature of the search function. (i.e. void(*)(int))
     * @param function_name The name of the searched function.
     * @return A function pointer to the found function symbol in the plugin.
     * @throw std::runtime_error If the symbol is not found or if a plugin is not loaded by this instance.
     * @warning There is no guarantee that the function has the wanted signature.
     */
    template <typename FunctionSignatureType>
        requires std::is_pointer_v<FunctionSignatureType>
                 && std::is_function_v<std::remove_cvref_t<decltype(*std::declval<FunctionSignatureType>)>>
    FunctionSignatureType find_function_ptr(std::string_view function_name)
    {
        return reinterpret_cast<FunctionSignatureType>(this->find_symbol_pointer(std::string(function_name)));
    }
};

}
}
