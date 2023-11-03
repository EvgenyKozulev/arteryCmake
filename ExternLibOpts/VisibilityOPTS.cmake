cmake_minimum_required(VERSION 3.15)

set(VisFreeRTOSConfih "${CMAKE_SOURCE_DIR}/ExternLibOpts/FreeRtosConfig")

target_include_directories(${PROJECT_NAME}.elf PUBLIC "${VisFreeRTOSConfih}")
