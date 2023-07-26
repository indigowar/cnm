set(CNM_CORE_includes ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE CNM_CORE_sources src/**.cpp)
file(GLOB_RECURSE CNM_CORE_headers src/**.hpp)
file(GLOB_RECURSE CNM_CORE_tests test/**.cpp)

add_library(cnm_core STATIC ${CNM_CORE_headers} ${CNM_CORE_sources})

target_include_directories(cnm_core PUBLIC ${CNM_CORE_includes})
target_link_libraries(cnm_core spdlog::spdlog)

enable_testing()

add_executable(cnm_core_tests ${CNM_CORE_tests})
target_include_directories(cnm_core_tests PUBLIC ${CNM_CORE_includes} GTest)
target_link_libraries(cnm_core_tests PUBLIC cnm_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cnm_core_tests)
