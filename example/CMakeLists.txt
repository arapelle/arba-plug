include(cmtk/CppLibraryExamples)

add_subdirectory(intgen)

add_cpp_library_example(plugin_example ${PROJECT_TARGET_NAME}
    SOURCES
        plugin_example.cpp
)
target_compile_definitions(plugin_example PUBLIC PLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}/intgen/libintgen")
