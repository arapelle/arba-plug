include(cmtk/CppLibraryTests)
include(GoogleTest)

add_subdirectory(concat_interface)
add_subdirectory(concat)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_test(safe_plugin_tests ${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        safe_plugin_tests.cpp
)
target_link_libraries(safe_plugin_tests PUBLIC arba_plug_concat_interface)
target_compile_definitions(safe_plugin_tests PUBLIC PLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}/concat/libarba_plug_concat")

add_cpp_library_test(plugin_tests ${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        plugin_tests.cpp
)
target_link_libraries(plugin_tests PUBLIC arba_plug_concat_interface)
target_compile_definitions(plugin_tests PUBLIC PLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}/concat/libarba_plug_concat")

add_cpp_library_basic_tests(${PROJECT_TARGET_NAME} GTest::gtest_main
    SOURCES
        project_version_tests.cpp
)
