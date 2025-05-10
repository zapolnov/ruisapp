cmake_minimum_required(VERSION 3.30)

set(name ruisapp)
project(${name})

if(NOT IGAGIS_CMAKE_COMMON_INCLUDED)
    set(dir "${CMAKE_CURRENT_SOURCE_DIR}/../../tool-configs/cmake")
    if(NOT EXISTS "${dir}/common.cmake")
        if(DEFINED ENV{IGAGIS_TOOL_CONFIGS})
            set(url "$ENV{IGAGIS_TOOL_CONFIGS}")
        else()
            set(url "https://github.com/cppfw/tool-configs")
        endif()
        file(MAKE_DIRECTORY "${dir}")
        file(DOWNLOAD "${url}/raw/refs/heads/main/cmake/common.cmake" "${dir}/common.cmake")
    endif()
    include("${dir}/common.cmake")
endif()

set(srcs)
add_source_directory(srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/${name}"
    PATTERNS "*.cpp" "*.hpp"
    RECURSIVE
)

declare_library(${name}-opengl
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
    INSTALL_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../src/${name}"
    PUBLIC_COMPILE_DEFINITIONS
        "RUISAPP_RENDER_OPENGL"
    DEPENDENCIES
        ruis
        ruis-render-opengl
    EXTERNAL_DEPENDENCIES
        GLEW
)

declare_library(${name}-opengles
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
    INSTALL_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../src/${name}"
    PUBLIC_COMPILE_DEFINITIONS
        "RUISAPP_RENDER_OPENGLES"
    DEPENDENCIES
        ruis
        ruis-render-opengles
    EXTERNAL_DEPENDENCIES
        EGL
        GLESv2
)

if(NOT RUISAPP_DISABLE_EXAMPLE)
    set(app_srcs)
    add_source_directory(app_srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/../../tests/app/src"
        PATTERNS "*.cpp" "*.hpp"
        RECURSIVE
    )

    set(app_rsrs)
    add_resource_directory(app_rsrs
        "${CMAKE_CURRENT_SOURCE_DIR}/../../tests/app/res"
        )

    declare_executable(${name}-test
        SOURCES
            ${app_srcs}
            ${app_rsrs}
        DEPENDENCIES
            ruis
        LINK_LIBRARIES
            ruisapp-opengl
    )
endif()
