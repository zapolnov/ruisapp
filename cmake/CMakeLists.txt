cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

set(name ruisapp)

project(${name})

if(NOT TARGET ruis::ruis)
find_package(ruis CONFIG REQUIRED)
endif()

if(NOT TARGET ruis-render-opengl::ruis-render-opengl)
find_package(ruis-render-opengl CONFIG REQUIRED)
endif()

set(srcdir "${CMAKE_CURRENT_SOURCE_DIR}/../src/${name}")
file(GLOB_RECURSE src RELATIVE "${srcdir}" CONFIGURE_DEPENDS "${srcdir}/*.cpp" "${srcdir}/*.hpp")

set(srcs)
foreach(file ${src})
    get_filename_component(path "${file}" DIRECTORY)
    string(REPLACE "/" "\\" path "Source Files/${path}")
    source_group("${path}" FILES "${srcdir}/${file}")
    list(APPEND srcs "${srcdir}/${file}")
endforeach()

add_library(
    ${name}
    STATIC
        ${srcs}
)

target_compile_features(${name} PUBLIC cxx_std_20)
set_target_properties(${name} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${name} PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(
    ${name}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${name}
    PUBLIC
        ruis::ruis
        ruis-render-opengl::ruis-render-opengl
)

if(NOT RUISAPP_DISABLE_EXAMPLE)

set(app_srcdir "${CMAKE_CURRENT_SOURCE_DIR}/../tests/app/src")
file(GLOB_RECURSE app_src RELATIVE "${app_srcdir}" CONFIGURE_DEPENDS "${app_srcdir}/*.cpp" "${app_srcdir}/*.hpp")

set(app_srcs)
foreach(file ${app_src})
    get_filename_component(path "${file}" DIRECTORY)
    string(REPLACE "/" "\\" path "Source Files/${path}")
    source_group("${path}" FILES "${app_srcdir}/${file}")
    list(APPEND app_srcs "${app_srcdir}/${file}")
endforeach()

add_executable(${name}-test
    ${app_srcs}
)

target_link_libraries(${name}-test
    PUBLIC
        ruis::ruis
        ruisapp::ruisapp
    )

endif()

if(NOT RUISAPP_DISABLE_INSTALL)

# install library header files preserving directory hierarchy
install(
    DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/${name}"
    DESTINATION
        "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN
        "*.hpp"
)

install(
    TARGETS
        ${name}
    EXPORT # generate cmake configs
        ${name}-config
)

# install cmake configs
install(
    EXPORT
        ${name}-config
    FILE
        ${name}-config.cmake
    DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${name}
    NAMESPACE
        ${name}::
)

endif()
