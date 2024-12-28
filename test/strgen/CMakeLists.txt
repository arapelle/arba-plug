cmake_minimum_required(VERSION 3.26)

include(CMakePrintHelpers)

add_library(arba_plug_strgen SHARED strgen.cpp)
target_link_libraries(arba_plug_strgen PUBLIC ${PROJECT_TARGET_NAME})
target_compile_features(arba_plug_strgen PUBLIC cxx_std_20)
set_property(TARGET arba_plug_strgen PROPERTY POSITION_INDEPENDENT_CODE 1)
