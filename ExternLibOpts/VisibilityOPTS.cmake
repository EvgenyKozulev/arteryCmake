cmake_minimum_required(VERSION 3.15)

set(VisFreeRTOSConfih "${CMAKE_SOURCE_DIR}/ExternLibOpts/FreeRtosConfig")
set(VisLWIP "${CMAKE_SOURCE_DIR}/githublibs/lwip/src/include/"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/atEmac"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting")

target_include_directories(${PROJECT_NAME}.elf PUBLIC
    "${VisFreeRTOSConfih}"
    "${VisLWIP}")
