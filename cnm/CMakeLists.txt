set(cnm_lib_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/cnm/src/)

file(GLOB_RECURSE cnm_lib_SRC_FILES src/**.cpp)

file(GLOB_RECURSE cnm_lib_HEADERS src/**.hpp)

file(GLOB_RECURSE cnm_lib_TEST_FILES test/**.cpp)

add_library(cnm_lib STATIC ${cnm_lib_HEADERS} ${cnm_lib_SRC_FILES})
target_include_directories(cnm_lib PUBLIC ${cnm_lib_INCLUDE_DIRECTORIES})

enable_testing()

add_executable(cnm_lib_tests ${cnm_lib_TEST_FILES})
target_include_directories(cnm_lib PUBLIC ${cnm_lib_INCLUDE_DIRECTORIES} GTest)
target_link_libraries(cnm_lib_tests PUBLIC cnm_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cnm_lib_tests)
