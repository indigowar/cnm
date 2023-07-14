add_executable(drawings drawings.hpp main.cpp)

target_include_directories(
  drawings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${IMGUI_INCLUDE_DIR}
                  ${Vulkan_INCLUDE_DIRS})
target_link_libraries(drawings PUBLIC imgui spdlog::spdlog ${Vulkan_LIBRARIES})
