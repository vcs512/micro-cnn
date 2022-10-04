cmake_minimum_required(VERSION 3.16.0)
list(APPEND EXTRA_COMPONENT_DIRS "lib/" "static_images/")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(micro-cnn)

target_add_binary_data(micro-cnn.elf "static_images/sample_images/image0" BINARY)
target_add_binary_data(micro-cnn.elf "static_images/sample_images/image1" BINARY)