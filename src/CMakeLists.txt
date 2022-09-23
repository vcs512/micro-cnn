# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(
    # SRCS ${app_sources}
    SRCS 
        "detection_responder.cc"
        "main.cc"
        "main_functions.cc"
        "model_settings.cc"
        "person_detect_model_data.cc"
        "esp_cli.c"

    # PRIV_REQUIRES console tflite-lib esp32-camera screen static_images spi_flash fb_gfx
    PRIV_REQUIRES console tflite-lib 
    INCLUDE_DIRS "")
