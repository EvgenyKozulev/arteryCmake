cmake_minimum_required(VERSION 3.15)

set(LWIP_DIR "${CMAKE_SOURCE_DIR}/githublibs/lwip")
set(LWIP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port/arch"
    "${CMAKE_SOURCE_DIR}/githublibs/lwip/src/include"
    "${CMAKE_SOURCE_DIR}/githublibs/lwip/contrib")

include("${CMAKE_SOURCE_DIR}/githublibs/lwip/src/Filelists.cmake")

# FreeRTOS port перенесен в папку lwipSettings
# include("${CMAKE_SOURCE_DIR}/githublibs/lwip/contrib/Filelists.cmake")
set(EthernetPortSrc "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/atEmac/at32_emac.c"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port/ethernetif.c"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port/sys_arch.c")

add_library(EthernetArtery "${EthernetPortSrc}")
target_link_libraries(EthernetArtery lwipcore)
target_link_libraries(EthernetArtery lwipallapps)
target_link_libraries(EthernetArtery lwipmbedtls)
target_link_libraries(EthernetArtery ArteryPlatform)

set(IncludePath "${LWIP_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/cmsis/cm4/device_support"
    "${CMAKE_SOURCE_DIR}/githublibs/lwip/src/include/lwip")
target_include_directories(EthernetArtery PRIVATE "${IncludePath}")
