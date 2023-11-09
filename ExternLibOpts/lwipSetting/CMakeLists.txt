cmake_minimum_required(VERSION 3.15)

set(LWIP_DIR "${CMAKE_SOURCE_DIR}/githublibs/lwip")
set(lwipOptsPath "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting")

set(hEmakPath "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/hEmac")
set(PortPath "${CMAKE_SOURCE_DIR}/githublibs/lwip/contrib/ports/freertos/include"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port")

set(LWIP_INCLUDE_DIRS
    "${LWIP_DIR}/src/include"
    "${LWIP_DIR}/contrib"
    "${PortPath}"
    "${lwipOptsPath}"
)
set(LWIP_CONTRIB_DIR "${CMAKE_SOURCE_DIR}/githublibs/lwip/contrib")

set(EmacSrc "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/hEmac/at32_emac.c"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/hEmac/netconf.c"
    "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/hEmac/telnet.c"
)
set(PortSrc "${CMAKE_SOURCE_DIR}/ExternLibOpts/lwipSetting/port/ethernetif.c"
     "${CMAKE_SOURCE_DIR}/githublibs/lwip/contrib/ports/freertos/sys_arch.c"
)
set(lwipDPath "${CMAKE_SOURCE_DIR}/githublibs/lwip/src/include/lwip")
# set(LWIP_DEFINITIONS LWIP_DEBUG=1)

include(${LWIP_DIR}/src/Filelists.cmake)
# include(${LWIP_DIR}/contrib/Filelists.cmake)

add_library(hEthernet "${PortSrc}" "${EmacSrc}")

target_include_directories(hEthernet PUBLIC
    "${LWIP_INCLUDE_DIRS}"
    "${hEmakPath}"
    "${PortPath}"
    "${lwipDPath}")

target_link_libraries(hEthernet lwipallapps lwipcore ArteryPlatform freertos_kernel)
# target_link_libraries(hEthernet lwipcontribapps lwipallapps lwipcore ArteryPlatform freertos_kernel)

target_link_libraries(lwipallapps hEthernet)
target_link_libraries(lwipcore hEthernet)

# target_link_libraries(lwipcontribapps hEthernet)

# target_link_libraries(ArteryPlatform hEthernet)
