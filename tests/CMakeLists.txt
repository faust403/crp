include(FetchContent)
FetchContent_Declare (
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    FIND_PACKAGE_ARGS NAMES GTest
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
        add_executable(${TESTNAME} ${TEST})

        target_link_libraries (
            ${TESTNAME}
            PRIVATE
                GTest::gtest
                ${TARGET_FOR_TEST}
        )
    endforeach()
endfunction()

GenerateTests("crp::hashes" "tests/hash")