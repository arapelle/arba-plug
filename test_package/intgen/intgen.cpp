#include <arba/plug/safe_plugin.hpp>

extern "C" int generate_int()
{
    return 642;
}

ARBA_PLUG_BEGIN_SAFE_PLUGIN_FUNCTION_REGISTER()
ARBA_PLUG_REGISTER_SAFE_PLUGIN_FUNCTION(generate_int)
ARBA_PLUG_END_SAFE_PLUGIN_FUNCTION_REGISTER()
