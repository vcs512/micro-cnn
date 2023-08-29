# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(
    # SRCS ${app_sources}
    SRCS 
        "main.cc"
        "main_functions.cc"
        "model_settings.cc"
        "model_data.cc"
        "esp_cli.c"
        "images.cc"

    PRIV_REQUIRES console tflite-lib 
    INCLUDE_DIRS "")
