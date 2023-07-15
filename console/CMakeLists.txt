add_executable(console main.cpp)

target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/cnm/src)
target_link_libraries(console PUBLIC spdlog::spdlog)
