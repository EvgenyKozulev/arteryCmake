cmake_minimum_required(VERSION 3.15)

set(hUsartSRC "${CMAKE_CURRENT_SOURCE_DIR}/intLib/SysComponets/hUsart/hUsart.c")

set(hNetSRC "${CMAKE_CURRENT_SOURCE_DIR}/intLib/SysComponets/hNet/netconf.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/intLib/SysComponets/hNet/tcp_client.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/intLib/SysComponets/hNet/at32f435_437_int.c")


add_library(HalLib "${hUsartSRC}"
    "${hNetSRC}")

target_link_libraries(HalLib ArteryPlatform)
target_link_libraries(HalLib EthernetArtery)

target_include_directories(HalLib PUBLIC "${CMAKE_SOURCE_DIR}/intLib/SysComponets")

