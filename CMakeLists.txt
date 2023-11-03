cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

set(CMAKE_C_COMPILER_WORKS ON)
set(CMAKE_CXX_COMPILER_WORKS ON)

# Установка компилятора и пути к инструментам
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)
set(CPU_FLAG "-DAT32F437ZMT7")

# set(CMAKE_C_GDB )
project(Artery VERSION 0.1.0 LANGUAGES C ASM)

set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/ArteryPlatform/linker/AT32F437xM_FLASH.ld")
set(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 ${CPU_FLAG} -g -Wall")

# Опции компилятора
# flag -Wcast-align - critical error from freeRTOS
set(Warnings "-Wextra -pipe -Wshadow -Wpointer-arith -Wbad-function-cast -Wsign-compare -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${Warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -T${LINKER_SCRIPT}")
message("CMAKE_C_FLAGS::${CMAKE_C_FLAGS}")

set(NewLibSrc "${CMAKE_SOURCE_DIR}/NewLibs/newLibs.c")

set(SRC_LIST "${CMAKE_SOURCE_DIR}/main/main.c"
    "${NewLibSrc}")
add_executable(${PROJECT_NAME}.elf ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}.elf ArteryPlatform)
add_subdirectory("${CMAKE_SOURCE_DIR}/ArteryPlatform")

# Опции дополнительной сборки
add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJDUMP} -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.S
    COMMAND ${CMAKE_NM} -n ${PROJECT_NAME}.elf > ${PROJECT_NAME}.map
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)

# Add custom target for debugging with OpenOCD
# set(OpenOCD_Bin_Path "c:/Program Files/AT32IDE/OpenOCD/bin")
# set(OpenOCD_Scripts_Path "c:/Program Files/AT32IDE/OpenOCD/scripts/")
# add_custom_target(debug
# COMMAND ${OpenOCD_Bin_Path}/openocd.exe -s ${OpenOCD_Scripts_Path} -f interface/atlink.cfg -f target/at32f437xM.cfg
# DEPENDS ${PROJECT_NAME}.elf
# COMMENT "Starting debugging session with OpenOCD"
# )

# Add custom target for flashing the program with OpenOCD
add_custom_target(flash
    COMMAND ${OpenOCD_Bin_Path}/openocd.exe -s ${OpenOCD_Scripts_Path} -f interface/atlink.cfg -f target/at32f437xM.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing ${PROJECT_NAME}.elf with OpenOCD"
)

# добавляет к проекту FreeRTOS
add_library(freertos_config INTERFACE)
set(freertos_include "${CMAKE_SOURCE_DIR}/ExternLibOpts/FreeRtosConfig"
    "${CMAKE_SOURCE_DIR}/githublibs/AT32F435_437_Firmware_Library/libraries/cmsis/cm4/device_support")
target_include_directories(freertos_config SYSTEM INTERFACE "${freertos_include}") # The config file directory
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "" FORCE)
target_link_libraries(${PROJECT_NAME}.elf freertos_kernel)
add_subdirectory("${CMAKE_SOURCE_DIR}/githublibs/FreeRTOS/FreeRTOS/Source")
include("${CMAKE_SOURCE_DIR}/ExternLibOpts/VisibilityOPTS.cmake")
