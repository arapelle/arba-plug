#include <arba/plug/plugin.hpp>

extern "C" int generate_int()
{
    return 642;
}

ARBA_PLUG_BEGIN_PLUGIN_FUNCTION_REGISTER()
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(generate_int)
ARBA_PLUG_END_PLUGIN_FUNCTION_REGISTER()
