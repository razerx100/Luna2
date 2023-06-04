cmake_minimum_required(VERSION 3.21)

project(Luna2
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_TYPE SHARED CACHE STRING "Defines the library type")

if(PROJECT_IS_TOP_LEVEL OR LIBRARY_TYPE STREQUAL "STATIC")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
endif()

file(GLOB_RECURSE SRC src/*.cpp src/*.ixx includes/*.hpp exports/*.hpp)
file(GLOB_RECURSE MODULESZ interfaces/*.ixx)

add_library(${PROJECT_NAME} ${LIBRARY_TYPE}
    ${SRC} ${MODULESZ}
)

source_group("Interfaces" FILES ${MODULESZ})

target_include_directories(${PROJECT_NAME} PRIVATE includes/ includes/Exceptions/ exports/)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:_DEBUG>" "$<$<CONFIG:RELEASE>:NDEBUG>" BUILD_LUNA)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Xinput.lib
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /fp:fast /MP /EHa /Ot /W4 /Gy)
endif()

if(PROJECT_IS_TOP_LEVEL)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()