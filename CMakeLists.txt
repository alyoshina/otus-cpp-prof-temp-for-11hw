cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

set(PRO_NAME "join_server")

project(${PRO_NAME} VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)
find_package(Boost CONFIG COMPONENTS program_options REQUIRED)

add_executable(${PRO_NAME} src/main.cpp src/parser.cpp src/data.cpp sqlite/sqlite3.c)

#target_link_libraries(${PRO_NAME} async Boost::program_options)
target_link_libraries(${PRO_NAME} Boost::program_options)

target_include_directories(${PRO_NAME} PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
    sqlite
)

set_target_properties(${PRO_NAME} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
#    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    target_compile_options(${PRO_NAME} PRIVATE 
#        -g
#        -Wall
#        -Wextra
#        -Werror
#        -pedantic
#    )
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    target_compile_options(${PRO_NAME} PRIVATE
#        /EHsc
#        /W4
#        /WX
#    )
#endif()

install(TARGETS ${PRO_NAME})

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

