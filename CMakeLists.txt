cmake_minimum_required(VERSION 3.16)

project(OpenGLApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios base
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(SRC_DIR ${ROOT_DIR}/src)
set(EXTERNAL_DIR ${ROOT_DIR}/external)

# Evitar configurar dentro del directorio fuente
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Usa siempre: cmake -S . -B build")
endif()

# Directorios de salida
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Validación de dependencias
if(NOT EXISTS ${SRC_DIR})
    message(FATAL_ERROR "No existe src/")
endif()

if(NOT EXISTS ${INCLUDE_DIR})
    message(FATAL_ERROR "No existe include/")
endif()

if(NOT EXISTS ${EXTERNAL_DIR}/glfw)
    message(FATAL_ERROR "No existe external/glfw")
endif()

if(NOT EXISTS ${EXTERNAL_DIR}/glad/src/glad.c)
    message(FATAL_ERROR "No existe external/glad/src/glad.c")
endif()

# GLFW (trae su propio CMakeLists.txt)
add_subdirectory(${EXTERNAL_DIR}/glfw)

# Glad (lo compilas como librería)
add_library(glad ${EXTERNAL_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${EXTERNAL_DIR}/glad/include)

# Archivos fuente y cabeceras
file(GLOB_RECURSE CPP_FILES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE H_FILES ${INCLUDE_DIR}/*.h)

add_executable(OpenGLApp ${CPP_FILES} ${H_FILES})

source_group("Source Files" FILES ${CPP_FILES})
source_group("Header Files" FILES ${H_FILES})

# Includes
target_include_directories(OpenGLApp PRIVATE
    ${INCLUDE_DIR}
    ${EXTERNAL_DIR}/glad/include
    ${EXTERNAL_DIR}/glfw/include
    ${EXTERNAL_DIR}/glm
)

# Linkeo multiplataforma
if (WIN32)
    target_link_libraries(OpenGLApp PRIVATE glad glfw opengl32)
elseif(APPLE)
    find_library(COCOA Cocoa REQUIRED)
    find_library(IOKIT IOKit REQUIRED)
    find_library(COREVIDEO CoreVideo REQUIRED)

    target_link_libraries(OpenGLApp PRIVATE
        glad glfw
        ${COCOA} ${IOKIT} ${COREVIDEO}
    )
else() # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(OpenGLApp PRIVATE glad glfw OpenGL::GL dl pthread X11)
endif()
