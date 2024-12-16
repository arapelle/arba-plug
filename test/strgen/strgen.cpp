#include <arba/plug/smart_plugin.hpp>
#include <string>

extern "C" std::string generate_str()
{
    return std::string("str");
}

ARBA_PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER()
ARBA_PLUG_REGISTER_SMART_PLUGIN_FUNCTION(generate_str)
ARBA_PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER()
