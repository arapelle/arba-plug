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

#ifdef NDEBUG
#define ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER()
#define ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION(function_)
#define ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER()
#else
#define ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER() ARBA_PLUG_BEGIN_SAFE_PLUGIN_FUNCTION_REGISTER()
#define ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION(function_) ARBA_PLUG_REGISTER_SAFE_PLUGIN_FUNCTION(function_)
#define ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER() ARBA_PLUG_END_SAFE_PLUGIN_FUNCTION_REGISTER()
#endif

}
}
