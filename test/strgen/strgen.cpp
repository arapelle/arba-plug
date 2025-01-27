#include <arba/plug/smart_plugin.hpp>

#include <string>

extern "C" std::string generate_str()
{
    return std::string("str");
}

PLUG_BEGIN_SMART_PLUGIN_FUNCTION_REGISTER()
PLUG_REGISTER_SMART_PLUGIN_FUNCTION(generate_str)
PLUG_END_SMART_PLUGIN_FUNCTION_REGISTER()
