cmake_minimum_required(VERSION 3.26)

include(CMakePrintHelpers)

add_library(intgen SHARED intgen.cpp)
target_link_libraries(intgen PUBLIC ${lib_target})
target_compile_features(intgen PUBLIC cxx_std_20)
set_property(TARGET intgen PROPERTY POSITION_INDEPENDENT_CODE 1)
