FetchContent_Declare (
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    FIND_PACKAGE_ARGS NAMES GTest
    CONFIGURE_COMMAND "cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release"
    BUILD_COMMAND "make -j32"
)
FetchContent_GetProperties(googletest)
FetchContent_MakeAvailable(googletest)

function(GenerateTests TARGET_FOR_TEST TESTS_PATH)
    file(GLOB TESTS "${PROJECT_SOURCE_DIR}/${TESTS_PATH}/*.cpp")
    list(LENGTH TESTS TESTS_LENGTH)
    if(TESTS_LENGTH EQUAL 0)
        message(FATAL_ERROR "${PROJECT_SOURCE_DIR}/${TESTS_PATH} is empty")
    endif()

    foreach(TEST ${TESTS})
        cmake_path(GET TEST STEM TESTNAME)
        add_executable(test${TESTNAME} ${TEST})

        target_link_libraries (
            test${TESTNAME}
            PRIVATE
                GTest::gtest
                ${TARGET_FOR_TEST}
        )
    endforeach()
endfunction()

GenerateTests("crp::hashes" "tests/hash")