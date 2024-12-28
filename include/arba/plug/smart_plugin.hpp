#pragma once

#ifdef NDEBUG
#include "plugin.hpp"
#else
#include "safe_plugin.hpp"
#endif

inline namespace arba
{
namespace plug
{

using smart_plugin =
#ifdef NDEBUG
    plugin
#else
    safe_plugin
#endif
    ;

} // namespace plug
} // namespace arba

#ifdef NDEBUG
#define ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER()
#define ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION(function_)
#define ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER()
#else
#define ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER() ARBA_PLUG_BEGIN_SAFE_PLUGIN_FUNCTION_REGISTER()
#define ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION(function_) ARBA_PLUG_REGISTER_SAFE_PLUGIN_FUNCTION(function_)
#define ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER() ARBA_PLUG_END_SAFE_PLUGIN_FUNCTION_REGISTER()
#endif

#ifndef PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER
#define PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER() ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER()
#else
#if not defined(NDEBUG) && (defined(__GNUC__) || defined(__GNUG__) || defined(_MSC_VER) || defined(__clang__))
#pragma message                                                                                                        \
    "PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER already exists. You must use ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER."
#endif
#endif

#ifndef PLUG_REGISTER_SMART_PLUGIN_FUNCTION
#define PLUG_REGISTER_SMART_PLUGIN_FUNCTION(function_) ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION(function_)
#else
#if not defined(NDEBUG) && (defined(__GNUC__) || defined(__GNUG__) || defined(_MSC_VER) || defined(__clang__))
#pragma message                                                                                                        \
    "PLUG_REGISTER_SMART_PLUGIN_FUNCTION already exists. You must use ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION."
#endif
#endif

#ifndef PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER
#define PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER() ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER()
#else
#if not defined(NDEBUG) && (defined(__GNUC__) || defined(__GNUG__) || defined(_MSC_VER) || defined(__clang__))
#pragma message                                                                                                        \
    "PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER already exists. You must use ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER."
#endif
#endif
