# This is the project CMakeLists.txt file for the test subproject
cmake_minimum_required(VERSION 3.16)

# Include the components directory of the main application:
#
set(EXTRA_COMPONENT_DIRS ../components)

# Set the components to include the tests for.
# This can be overriden from CMake cache:
# - when invoking CMake directly: cmake -D TEST_COMPONENTS="xxxxx" ..
# - when using idf.py: idf.py -T xxxxx build
#
set(TEST_COMPONENTS "lora" CACHE STRING "List of components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(unit_test_test)

add_subdirectory(../external/fff ${CMAKE_BINARY_DIR}/external/fff)

# Add here if the component is compatible with IDF >= v5.3
if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER_EQUAL "5.3")
    #list(APPEND EXTRA_COMPONENT_DIRS ../esp_lcd_qemu_rgb)
endif()