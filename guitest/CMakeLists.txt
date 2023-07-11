include(${CMAKE_SOURCE_DIR}/cmake/get_imgui.cmake)
add_executable(gui_test work.hpp main.cpp)
target_include_directories(gui_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${IMGUI_INCLUDE_DIR} ${Vulkan_INCLUDE_DIRS} )
target_link_libraries(gui_test PUBLIC imgui spdlog::spdlog ${Vulkan_LIBRARIES})

add_executable(gui_test_scenes scenes_main.cpp)
target_include_directories(gui_test_scenes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${IMGUI_INCLUDE_DIR} ${Vulkan_INCLUDE_DIRS} )
target_link_libraries(gui_test_scenes PUBLIC imgui spdlog::spdlog ${Vulkan_LIBRARIES})

