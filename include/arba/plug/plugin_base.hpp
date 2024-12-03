#pragma once

#include "exception.hpp"

#include <filesystem>

inline namespace arba
{
namespace plug
{

constexpr std::string_view plugin_file_extension =
#ifdef WIN32
    ".dll";
#else
    ".so";
#endif

/**
 * @brief The plugin_base class
 */
class plugin_base
{
protected:
    inline plugin_base() {}

    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::runtime_error If the file does not exist or if there is a problem during loading.
     */
    explicit plugin_base(const std::filesystem::path& plugin_path);

public:
    /**
     * @brief ~Plugin destructor which unloads the plugin.
     * @warning If any error occurs while unloading, an error message is written
     * on the error stream and the program exit with the error code.
     */
    ~plugin_base();

    /**
     * @brief plugin_base Move constructor.
     * @param other A r-value plugin instance.
     * The embedded handle of the other instance is set to nullptr.
     */
    plugin_base(plugin_base&& other);

    /**
     * @brief operator = Move assignment.
     * @param other A r-value plugin instance.
     * @return A reference to the current instance.
     * The embedded handle of the other instance is set to nullptr.
     */
    plugin_base& operator=(plugin_base&& other);

    /**
     * @brief load_from_file Load the plugin present at a given path.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::runtime_error If the file does not exist or if there is a problem during loading.
     * @warning If a plugin is already loaded by this instance, the behavior is undefined.
     */
    void load_from_file(const std::filesystem::path& plugin_path);

    /**
     * @brief unload Unload the plugin.
     * @warning If no plugin is loaded by this instance, the behavior is undefined.
     */
    void unload();

    /**
     * @brief is_loaded Indicate is this plugin is loaded or not.
     * @return true If a loaded plugin is held by this instance.
     */
    [[nodiscard]] inline bool is_loaded() const noexcept { return handle_; }

protected:
    void* find_symbol_pointer(const std::string& symbol_name);

private:
    plugin_base(const plugin_base&) = delete;
    plugin_base& operator=(const plugin_base&) = delete;

protected:
    void* handle_ = nullptr;
};

}
}
