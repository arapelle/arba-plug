#include <arba/plug/plugin_base.hpp>
#include <cassert>
#include <format>
#include <iostream>
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

inline namespace arba
{
namespace plug
{

// Windows API:
//   https://learn.microsoft.com/fr-fr/windows/win32/dlls/using-run-time-dynamic-linking
//   https://learn.microsoft.com/fr-fr/windows/win32/dlls/creating-a-simple-dynamic-link-library
// UNIX API (dl):
//   https://linux.die.net/man/3/dlopen

plugin_base::plugin_base(const std::filesystem::path& plugin_path)
{
    load_from_file(plugin_path);
}

plugin_base::~plugin_base()
{
    if (is_loaded())
    {
        try
        {
            unload();
        }
        catch (const std::runtime_error& err)
        {
            std::cerr << err.what() << std::endl;
            std::terminate();
        }
    }
}

plugin_base::plugin_base(plugin_base&& other)
{
    handle_ = other.handle_;
    other.handle_ = nullptr;
}

plugin_base& plugin_base::operator=(plugin_base&& other)
{
    if (&other != this)
    {
        if (handle_)
            unload();
        handle_ = other.handle_;
        other.handle_ = nullptr;
    }
    return *this;
}

void plugin_base::load_from_file(const std::filesystem::path& plugin_path)
{
    assert(!is_loaded());
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
    static_assert(std::is_pointer_v<HINSTANCE>);
    static_assert(std::is_nothrow_convertible_v<HINSTANCE, void*>);

    HINSTANCE instance = LoadLibraryW(plugin_path.native().c_str());
    if (!instance) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw plugin_load_error(
            error_code, std::format("Exception occurred while loading plugin: {}", plugin_path.generic_string()));
    }
    handle_ = static_cast<void*>(instance);
#else
    std::string plugin_path_string;
    if (plugin_path.has_extension() || std::filesystem::exists(plugin_path))
    {
        plugin_path_string = plugin_path.generic_string();
    }
    else
    {
        plugin_path_string = plugin_path.generic_string() + ".so";
    }
    void* handle = dlopen(plugin_path_string.c_str(), RTLD_LAZY);
    if (!handle) [[unlikely]]
    {
        std::string error_message(dlerror());
        throw plugin_load_error(std::format("Exception occurred while loading plugin: {}", error_message));
    }
    handle_ = handle;
#endif
}

void plugin_base::unload()
{
    assert(is_loaded());
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
    int result = FreeLibrary(static_cast<HINSTANCE>(handle_));
    if (result == 0) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw plugin_unload_error(error_code,
                                  std::format("A problem occured while unloading plugin: {}", error_code.message()));
    }
#else
    int result = dlclose(handle_);
    if (result != 0) [[unlikely]]
    {
        std::string error_message(dlerror());
        throw plugin_unload_error(std::format("A problem occured while unloading plugin: {}", error_message));
    }
#endif
    handle_ = nullptr;
}

void* plugin_base::find_symbol_pointer(const std::string& symbol_name)
{
    assert(is_loaded());
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
    static_assert(std::is_pointer_v<FARPROC>);

    FARPROC pointer = GetProcAddress(static_cast<HINSTANCE>(handle_), symbol_name.c_str());
    if (!pointer) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw plugin_find_symbol_error(error_code,
                                       std::format("Exception occurred while looking for address of {}", symbol_name));
    }
    return reinterpret_cast<void*>(pointer);
#else
    dlerror(); // Clear any existing error
    void* pointer = dlsym(handle_, symbol_name.c_str());
    if (!pointer) [[unlikely]]
    {
        std::string error_message(dlerror());
        throw plugin_find_symbol_error(
            std::format("Exception occurred while looking for address of symbol: {}", error_message));
    }
    return pointer;
#endif
}

} // namespace plug
} // namespace arba
