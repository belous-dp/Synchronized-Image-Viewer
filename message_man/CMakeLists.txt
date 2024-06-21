add_library(message_man STATIC
        message_man.cpp message_man.h
        write_client.cpp write_client.h)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

target_compile_options(message_man PRIVATE -Wall)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(message_man PRIVATE -Wno-sign-compare -Wshadow-uncaptured-local -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(message_man PRIVATE -Wno-sign-compare -Wshadow=compatible-local -pedantic)
endif ()

option(USE_SANITIZERS "Enable to build with undefined, leak and address sanitizers" OFF)
if (USE_SANITIZERS)
    target_compile_options(message_man PUBLIC -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
    target_link_options(message_man PUBLIC -fsanitize=address,undefined,leak)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(message_man PUBLIC -stdlib=libc++)
endif ()


if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(message_man PUBLIC -D_GLIBCXX_DEBUG)
endif ()

set_property(TARGET message_man PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(message_man PUBLIC Qt6::Core Qt6::Network)
