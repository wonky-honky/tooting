# SPDX-License-Identifier: Unlicense

cmake_minimum_required( VERSION 3.22 )

message( STATUS "Using CMake ${CMAKE_VERSION}" )
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

# Require out-of-source builds
file( TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH )

if ( EXISTS "${LOC_PATH}" )
    message( FATAL_ERROR "You cannot build in the source directory. Please use a build subdirectory." )
endif()

# Add paths to modules
list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )

# Turn on link time optimization for everything
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#set(BUILD_GUI=ON SYSTEM_FFTW=ON SYSTEM_FMT=OFF SYSTEM_LIBSNDFILE=ON SYSTEM_RTMIDI=ON SYSTEM_SDL2=ON SYSTEM_ZLIB=ON SYSTEM_PORTAUDIO=ON WITH_PORTAUDIO=ON WITH_RENDER_SDL=ON WITH_RENDER_OPENGL=ON)
#furnace
add_subdirectory(extern/furnace)

add_subdirectory(extern/rustex)
add_subdirectory(extern/ctre)
add_subdirectory(extern/frozen)
add_subdirectory(extern/GSL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()
add_compile_options("-fexperimental-library")
add_link_options("-fuse-ld=lld")
# Build universal lib on macOS
# Note that CMAKE_OSX_ARCHITECTURES must be set before project().
if ( APPLE )
    set( CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" )
endif()

# Main project information
project( tooting
    LANGUAGES
        CXX
    VERSION
        0.1.0
)

# Create our library
add_library( ${PROJECT_NAME} SHARED )

target_compile_features( ${PROJECT_NAME}
    PRIVATE
        cxx_std_23
)

include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/extern ${FMT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/extern/ctre/include ${PROJECT_SOURCE_DIR}/extern/frozen/include ${PROJECT_SOURCE_DIR}/extern/GSL/include)
# LIB_ARCH is the architecture being built. It is set to the build system's architecture.
# For macOS, we build a universal library (both arm64 and x86_64).
set( LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
if ( APPLE )
    set( LIB_ARCH "universal" )
endif()

# LIB_DIR is where the actual library ends up. This is used in both the build directory and the
# install directory and needs to be consistent with the paths in the gdextension file.
#   e.g. linux.release.x86_64 = "lib/Linux-x86_64/libGDExtensionTemplate.so"
set( LIB_DIR "lib/${CMAKE_SYSTEM_NAME}-${LIB_ARCH}" )

message( STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")

# BUILD_OUTPUT_DIR is where we put the resulting library (in the build directory)
set( BUILD_OUTPUT_DIR "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/" )

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
)

if( NOT DEFINED CMAKE_DEBUG_POSTFIX )
    set_target_properties( ${PROJECT_NAME}
        PROPERTIES
            DEBUG_POSTFIX "-d"
    )
endif()

# Copy over additional files from the support_files directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/support_files"
            ${BUILD_OUTPUT_DIR}
)
#if(LINUX)
#  add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy
#    $<TARGET_FILE:furnace>
#    "${BUILD_OUTPUT_DIR}/${LIB_DIR}/"
#  )
#endif()

file(MAKE_DIRECTORY "${BUILD_OUTPUT_DIR}/testres")
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  "${PROJECT_SOURCE_DIR}/extern/furnace/demos/genesis/MegadriveOverdrive.fur"
  "${BUILD_OUTPUT_DIR}/testres/"
)


# Warnings
include( CompilerWarnings )

# Create and include version info file from git
include( GitVersionInfo )

add_subdirectory( src )

# Install library, extension file, and support files in ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
set( INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/" )

message( STATUS "Install directory: ${INSTALL_DIR}")

set_target_properties(${PROJECT_NAME}
       PROPERTIES
       LINK_FLAGS "-Wl,-rpath,\\\$ORIGIN/../../../usr/lib:\\\$ORIGIN"
)
install( TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR}/${LIB_DIR})
install( TARGETS furnace DESTINATION usr/lib)
# Copy over support files
install( DIRECTORY "${CMAKE_SOURCE_DIR}/support_files/"
    DESTINATION ${INSTALL_DIR}
    PATTERN ".*" EXCLUDE
)

add_subdirectory( templates )

# ccache
# Turns on ccache if found
include( ccache )

# Formatting
# Adds a custom target to format all the code at once
include( ClangFormat )

# godot-cpp
# From here: https://github.com/godotengine/godot-cpp
if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/extern/godot-cpp/Makefile" )
    message(
        FATAL_ERROR
            "[${PROJECT_NAME}] The godot-cpp submodule was not downloaded. Please update submodules: git submodule update --init --recursive."
    )
endif()

set( GODOT_CPP_SYSTEM_HEADERS ON CACHE BOOL "" FORCE )

add_subdirectory( extern/godot-cpp )

set_target_properties( godot-cpp
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden    # visibility needs to be the same as the main library
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        godot-cpp
        fmt
        furnace
        rustex
        ctre
        frozen
        Microsoft.GSL::GSL
)
