#include "concat.hpp"

#include <arba/plug/plugin.hpp>
#include <format>
#include <iostream>

Concat::Concat() : Concat("", "")
{
}

Concat::Concat(std::string_view left_decorator, std::string_view right_decorator)
    : left_decorator_(left_decorator), right_decorator_(right_decorator)
{
}

std::string Concat::concat(std::string_view left_value, std::string_view right_value) const
{
    return std::format("{}{}-{}{}", left_decorator_, left_value, right_value, right_decorator_);
}

extern "C" std::unique_ptr<ConcatInterface> make_unique_instance()
{
    return std::make_unique<Concat>();
}

// The purpose is to test that all ways of providing an argument is working well with
// (unsafe_)plugin::make_unique_instance():
// - value copy with first_left_decorator
// - value reference with second_left_decorator
// - value const reference with right_decorator
extern "C" std::unique_ptr<ConcatInterface> make_unique_instance_from_args(std::string_view first_left_decorator,
                                                                           std::string& second_left_decorator,
                                                                           const std::string& right_decorator)
{
    second_left_decorator = std::string(first_left_decorator) + second_left_decorator;
    return std::make_unique<Concat>(second_left_decorator, right_decorator);
}

extern "C" std::shared_ptr<ConcatInterface> make_shared_instance()
{
    return std::make_shared<Concat>();
}

// The purpose is to test that all ways of providing an argument is working well with
// (unsafe_)plugin::make_shared_instance():
// - value copy with first_left_decorator
// - value reference with second_left_decorator
// - value const reference with right_decorator
extern "C" std::shared_ptr<ConcatInterface> make_shared_instance_from_args(std::string_view first_left_decorator,
                                                                           std::string& second_left_decorator,
                                                                           const std::string& right_decorator)
{
    second_left_decorator = std::string(first_left_decorator) + second_left_decorator;
    return std::make_shared<Concat>(second_left_decorator, right_decorator);
}

// The purpose is to test that all ways of providing an argument is working well with
// (unsafe_)plugin::find_function_ptr():
// - value copy with left_value
// - value reference with res
// - value const reference with right_value
extern "C" void execute(std::string& res, std::string_view left_value, const std::string& right_value)
{
    res = Concat().concat(left_value, right_value);
}

extern "C" ConcatInterface& default_concat()
{
    static Concat instance;
    return instance;
}

extern "C" const ConcatInterface& default_const_concat()
{
    static Concat instance;
    return instance;
}

extern "C" int unregistered_function(std::string_view)
{
    return 0;
}

ARBA_PLUG_BEGIN_PLUGIN_FUNCTION_REGISTER()
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(make_unique_instance)
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(make_unique_instance_from_args)
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(make_shared_instance)
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(make_shared_instance_from_args)
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(execute)
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(default_concat)
ARBA_PLUG_REGISTER_PLUGIN_FUNCTION(default_const_concat)
ARBA_PLUG_END_PLUGIN_FUNCTION_REGISTER()
