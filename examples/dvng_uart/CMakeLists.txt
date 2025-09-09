cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(EXTRA_COMPONENT_DIRS $ENV{DAVANG_COMPONENT_DIRS})

set(COMPONENTS main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project( davang_replier )
