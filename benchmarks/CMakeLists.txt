FetchContent_Declare (
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
    FIND_PACKAGE_ARGS NAMES benchmark
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