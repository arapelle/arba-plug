include(cmtk/CppLibraryExamples)

add_subdirectory(intgen)

add_cpp_library_example(safe_plugin_example ${PROJECT_TARGET_NAME}
    SOURCES
        safe_plugin_example.cpp
)
target_compile_definitions(safe_plugin_example PUBLIC PLUGIN_PATH="${CMAKE_CURRENT_BINARY_DIR}/intgen/libintgen")
