cmake_minimum_required(VERSION 3.15)

set(hUsartSRC "${CMAKE_CURRENT_SOURCE_DIR}/intLib/SysComponets/hUsart/hUsart.c")

add_library(HalLib "${hUsartSRC}"
)
target_link_libraries(HalLib ArteryPlatform)
target_include_directories(HalLib PUBLIC "${CMAKE_SOURCE_DIR}/intLib/SysComponets")
