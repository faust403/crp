FetchContent_Declare (
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
    FIND_PACKAGE_ARGS NAMES benchmark
    CONFIGURE_COMMAND "cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release"
    BUILD_COMMAND "make -j32"
)
FetchContent_GetProperties(benchmark)
FetchContent_MakeAvailable(benchmark)

# cryptopp
set(CRYPTOPP_CMAKE "cryptopp-cmake")
FetchContent_Declare(
    ${CRYPTOPP_CMAKE}
    GIT_REPOSITORY  https://github.com/noloader/cryptopp-cmake
    GIT_TAG         CRYPTOPP_8_2_0
)

FetchContent_GetProperties(${CRYPTOPP_CMAKE})
if(NOT ${CRYPTOPP_CMAKE}_POPULATED)
    FetchContent_Populate(${CRYPTOPP_CMAKE})
endif()

set(CRYPTOPP "cryptopp")
FetchContent_Declare(
    ${CRYPTOPP}
    GIT_REPOSITORY  https://github.com/weidai11/cryptopp
    GIT_TAG         CRYPTOPP_8_2_0
    CONFIGURE_COMMAND "cmake -S . -B build -DCRYPTOPP_DATA_DIR=\"${${CRYPTOPP}_SOURCE_DIR}/TestData\" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release"
)

FetchContent_GetProperties(${CRYPTOPP})
if(NOT ${CRYPTOPP}_POPULATED)
    FetchContent_Populate(${CRYPTOPP})
endif()

file(COPY ${${CRYPTOPP_CMAKE}_SOURCE_DIR}/CMakeLists.txt DESTINATION ${${CRYPTOPP}_SOURCE_DIR})
add_subdirectory(${${CRYPTOPP}_SOURCE_DIR} ${${CRYPTOPP}_BINARY_DIR})
include_directories(${${CRYPTOPP}_SOURCE_DIR})
set(CRYPTOPP_LIB ${${CRYPTOPP}_BINARY_DIR}/libcryptopp.so)

find_package(OpenSSL REQUIRED)
if(NOT OpenSSL_FOUND)
    message(FATAL_ERROR "Could not find openssl as package. Install it before doing benchmarks")
endif()

function(GenerateBenchmarks TARGET_FOR_BENCHMARK BENCHMARKS_PATH)
    file(GLOB BENCHMARKS "${PROJECT_SOURCE_DIR}/${BENCHMARKS_PATH}/*.cpp")
    list(LENGTH BENCHMARKS BENCHMARKS_LENGTH)
    if(BENCHMARKS_LENGTH EQUAL 0)
        message(FATAL_ERROR "${PROJECT_SOURCE_DIR}/${BENCHMARKS_PATH} is empty")
    endif()

    foreach(BENCHMARK ${BENCHMARKS})
        cmake_path(GET BENCHMARK STEM BENCHMARKNAME)
        add_executable(bench${BENCHMARKNAME} ${BENCHMARK})

        target_link_libraries (
            bench${BENCHMARKNAME}
            PRIVATE
                benchmark::benchmark
                OpenSSL::Crypto
                ${CRYPTOPP}-shared
                ${TARGET_FOR_BENCHMARK}
        )
    endforeach()
endfunction()

GenerateBenchmarks("crp::hashes" "benchmarks/hash")