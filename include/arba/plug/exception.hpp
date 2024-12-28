#pragma once

#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <system_error>
#else
#include <stdexcept>
#endif

inline namespace arba
{
namespace plug
{

class plugin_load_error : public
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
                          std::system_error
{
    using std::system_error::system_error;
#else
                          std::runtime_error
{
    using std::runtime_error::runtime_error;
#endif
};

class plugin_unload_error : public
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
                            std::system_error
{
    using std::system_error::system_error;
#else
                            std::runtime_error
{
    using std::runtime_error::runtime_error;
#endif
};

class plugin_find_symbol_error : public
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
                                 std::system_error
{
    using std::system_error::system_error;
#else
                                 std::runtime_error
{
    using std::runtime_error::runtime_error;
#endif
};

} // namespace plug
} // namespace arba
