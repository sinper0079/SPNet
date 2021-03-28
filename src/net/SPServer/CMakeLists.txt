cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME SPServer)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

sita_add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER sita/net)
target_link_libraries(${PROJECT_NAME} PUBLIC sita_core PUBLIC sita_net_common imgui::imgui)