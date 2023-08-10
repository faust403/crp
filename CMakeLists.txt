cmake_minimum_required(VERSION 3.8...3.26)
project(crp)

# Fallback for using newer policies on CMake <3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy (
        VERSION
        ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}
    )
endif()

option(CRP_HASHES  "Enable hash functions on" ON)
option(CRP_RANDOM  "Enable random functions on" ON)
option(CRP_TESTS   "Enable tests on" OFF)
option(CRP_DEBUG   "Enable debug options" ON)
option(CRP_RELEASE "Enable max optimizations on" ON)
option(CRP_WERROR  "Enable warning messages on" ON)

if(CRP_TESTS)
    message(STATUS "Tests enabled")
    enable_testing()
    add_subdirectory(tests)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(PEDANTIC_COMPILE_FLAGS -pedantic-errors -Wall -Wextra -pedantic
        -Wold-style-cast -Wundef
        -Wredundant-decls -Wwrite-strings -Wpointer-arith
        -Wcast-qual -Wformat=2 -Wmissing-include-dirs
        -Wcast-align
        -Wctor-dtor-privacy -Wdisabled-optimization
        -Winvalid-pch -Woverloaded-virtual
        -Wconversion -Wundef
        -Wno-ctor-dtor-privacy -Wno-format-nonliteral)
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
        set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS}
            -Wno-dangling-else -Wno-unused-local-typedefs)
    endif ()
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wdouble-promotion
            -Wtrampolines -Wzero-as-null-pointer-constant -Wuseless-cast
            -Wvector-operation-performance -Wsized-deallocation -Wshadow)
    endif ()
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wshift-overflow=2
            -Wnull-dereference -Wduplicated-cond)
    endif ()
    set(WERROR_FLAG -Werror)
endif ()

include(CheckCXXCompilerFlag)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(PEDANTIC_COMPILE_FLAGS -Wall -Wextra -pedantic -Wconversion -Wundef
        -Wdeprecated -Wweak-vtables -Wshadow
        -Wno-gnu-zero-variadic-macro-arguments)
    check_cxx_compiler_flag(-Wzero-as-null-pointer-constant HAS_NULLPTR_WARNING)
    if (HAS_NULLPTR_WARNING)
        set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS}
            -Wzero-as-null-pointer-constant)
    endif ()
    set(WERROR_FLAG -Werror)
endif ()

function(GenerateLibrary LIBRARY_NAME INCLUDE_PATH SOURCE_PATH)
    file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/${SOURCE_PATH}/*.cpp")
    list(LENGTH SOURCES SOURCES_LENGTH)
    if(SOURCES_LENGTH EQUAL 0)
        message(FATAL_ERROR "Invalid SOURCE_PATH given")
    endif()

    file(GLOB HEADERS "${PROJECT_SOURCE_DIR}/${INCLUDE_PATH}/*.hpp")
    list(LENGTH HEADERS HEADERS_LENGTH)
    if(HEADERS_LENGTH EQUAL 0)
        message(FATAL_ERROR "Invalid INCLUDE_PATH given")
    endif()

    add_library(${LIBRARY_NAME} ${SOURCES})
    add_library(crp::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
    set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

    target_include_directories (
        ${LIBRARY_NAME}
        PUBLIC
            ${HEADERS}
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    )

    target_compile_options (
        ${LIBRARY_NAME}
        PRIVATE
            ${PEDANTIC_COMPILE_FLAGS}
    )

    if(CRP_DEBUG)
        target_compile_options (
            ${LIBRARY_NAME}
            PRIVATE
                -ggdb -g3
        )
    endif()

    if(CRP_RELEASE)
        target_compile_options (
            ${LIBRARY_NAME}
            PRIVATE
                -Ofast
        )
    endif()

    if(CRP_WERROR)
        target_compile_options (
            ${LIBRARY_NAME}
            PRIVATE
                ${WERROR_FLAG}
        )
    endif()

    install(TARGETS ${LIBRARY_NAME} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/lib")
    message(STATUS "Library ${LIBRARY_NAME}(include: ${INCLUDE_PATH} source:${SOURCE_PATH})")
endfunction()

GenerateLibrary("hashes" "include/crp" "src/hash")