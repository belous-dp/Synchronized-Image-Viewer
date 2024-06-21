cmake_minimum_required(VERSION 3.25)
project(synchronized-image-viewer)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

add_subdirectory(message_man)

add_executable(synchronized-image-viewer
        main.cpp
        window.cpp
        view_area.cpp
        cursor.cpp
        setup.cpp
)

target_compile_options(synchronized-image-viewer PRIVATE -Wall)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(synchronized-image-viewer PRIVATE -Wno-sign-compare -Wshadow-uncaptured-local -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(synchronized-image-viewer PRIVATE -Wno-sign-compare -Wshadow=compatible-local -pedantic)
endif ()

option(USE_SANITIZERS "Enable to build with undefined, leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    target_compile_options(synchronized-image-viewer PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_link_options(synchronized-image-viewer PUBLIC -fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(synchronized-image-viewer PUBLIC -stdlib=libc++)
endif ()


if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(synchronized-image-viewer PUBLIC -D_GLIBCXX_DEBUG)
endif ()

set_property(TARGET synchronized-image-viewer PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(synchronized-image-viewer PUBLIC Qt6::Core Qt6::Widgets Qt6::Network message_man)
target_include_directories(synchronized-image-viewer PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/message_man"
)
