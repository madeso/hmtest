cmake_minimum_required(VERSION 3.1.0)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
project(hmtest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE "GLVND")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# compiler flags
if(${MSVC})
    add_definitions(/D _CRT_SECURE_NO_WARNINGS)

    add_definitions(/D NOMINMAX)
    add_definitions(/D STRICT)
    add_definitions(/D WIN32_LEAN_AND_MEAN)
    add_definitions(/D VC_EXTRALEAN)

    # character set to Not Set=ASCII/SBCS
    # http://public.kitware.com/Bug/view.php?id=12189
    add_definitions(/D _SBCS)
endif()

# enable all warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wshadow)
    add_compile_options(-Wnon-virtual-dtor)
    # add_compile_options(-Wold-style-cast) # todo...
    add_compile_options(-Wcast-align)
    add_compile_options(-Wunused)
    add_compile_options(-Woverloaded-virtual)
    #add_compile_options(-Wconversion)
    #add_compile_options(-Wsign-conversion)
    add_compile_options(-Wformat=2)
    # add_compile_options(-Wlifetime)

    add_compile_options(-Wno-error=deprecated)
endif()


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(hmtest)

