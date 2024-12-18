#include <arba/plug/safe_plugin.hpp>

extern "C" int generate_int()
{
    return 642;
}

PLUG_BEGIN_SAFE_PLUGIN_FUNCTION_REGISTER()
PLUG_REGISTER_SAFE_PLUGIN_FUNCTION(generate_int)
PLUG_END_SAFE_PLUGIN_FUNCTION_REGISTER()
