# add Dear ImGui include(${CMAKE_SOURCE_DIR}/cmake/get_imgui.cmake)

set(cnm_gui_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/)

file(GLOB_RECURSE cnm_gui_HEADERS src/**.hpp)
file(GLOB_RECURSE cnm_gui_SOURCES src/**.cpp)

# message("CNM_GUI given headers: ${cnm_gui_HEADERS}") message("CNM_GUI given
# sources: ${cnm_gui_SOURCES}")

add_executable(cnm_gui ${cnm_gui_HEADERS} ${cnm_gui_SOURCES})

target_include_directories(
  cnm_gui
  PUBLIC ${cnm_gui_INCLUDE_DIRS} ${CNM_CORE_includes} ${IMGUI_INCLUDE_DIR}
         ${Vulkan_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/cnm/src)
target_link_libraries(cnm_gui PUBLIC cnm_core imgui spdlog::spdlog
                                     ${Vulkan_LIBRARIES})

# message("Vulkan lib list:${Vulkan_LIBRARIES}")
