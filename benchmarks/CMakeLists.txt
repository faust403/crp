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
                ${TARGET_FOR_BENCHMARK}
        )
    endforeach()
endfunction()

GenerateBenchmarks("crp::hashes" "benchmarks/hash")