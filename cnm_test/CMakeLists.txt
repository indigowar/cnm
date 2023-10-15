add_executable(cnm_test main.cpp)
target_include_directories(cnm_test PUBLIC ${CNM_CORE_includes})
target_link_libraries(cnm_test PUBLIC cnm_core spdlog::spdlog)