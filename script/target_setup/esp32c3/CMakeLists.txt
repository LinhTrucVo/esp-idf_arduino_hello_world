# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

add_definitions(
-DARDUINO_USB_MODE=1
-DARDUINO_USB_CDC_ON_BOOT=1
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hello_world)
