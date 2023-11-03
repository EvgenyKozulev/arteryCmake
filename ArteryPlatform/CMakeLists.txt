cmake_minimum_required(VERSION 3.15)

set(ArteryStartup "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/cmsis/cm4/device_support/startup/gcc/startup_at32f435_437.s")
set(SystemArtery "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/cmsis/cm4/device_support/system_at32f435_437.c")
set(ArteryClock "${CMAKE_SOURCE_DIR}/ArteryPlatform/src/at32f435_437_clock.c")

set(CMSIS_PathArtery "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/cmsis/cm4/core_support/"
    "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/cmsis/cm4/device_support/"
    "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/drivers/inc/"
    "${CMAKE_SOURCE_DIR}/ArteryPlatform/inc/")

file(GLOB_RECURSE SPL_ArteryDrivers "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/drivers/src/*.c")



add_library(ArteryPlatform "${ArteryStartup}" "${SystemArtery}" "${ArteryClock}" "${SPL_ArteryDrivers}")
target_include_directories(ArteryPlatform PUBLIC "${CMSIS_PathArtery}")