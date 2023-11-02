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

set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/ExtLibs/artery_platform/linker/AT32F437xM_FLASH.ld")
set(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 ${CPU_FLAG} -g -Wall")

# Опции компилятора
# flag -Wcast-align - critical error from freeRTOS
set(Warnings "-Wextra -pipe -Wshadow -Wpointer-arith -Wbad-function-cast -Wsign-compare -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${Warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -T${LINKER_SCRIPT}")
message("CMAKE_C_FLAGS::${CMAKE_C_FLAGS}")
set(StabsNewLibSrc "${CMAKE_SOURCE_DIR}/ExtLibs/artery_platform/mztaSystemCallStubs/NewLibCallStabs.c")

set(SRC_LIST
    "${CMAKE_SOURCE_DIR}/main/main.c"
    "${StabsNewLibSrc}")
add_executable(${PROJECT_NAME}.elf ${SRC_LIST})

# Платформа Artery
add_subdirectory("${CMAKE_SOURCE_DIR}/ExtLibs/artery_platform")
target_link_libraries(${PROJECT_NAME}.elf ArteryPlatform)

# добавляет к проекту FreeRTOS
set(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_SOURCE_DIR}/ExtLibs/FreeRTOS_settings")
set(FREERTOS_PORT "GCC_ARM_CM4F")
target_link_libraries(${PROJECT_NAME}.elf freertos_kernel)
add_subdirectory("${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS/Source")

# добавляет lwip
add_subdirectory("${CMAKE_SOURCE_DIR}/ExtLibs/lwip_settings")
target_link_libraries(${PROJECT_NAME}.elf lwipcore)
target_link_libraries(${PROJECT_NAME}.elf lwipallapps)
target_link_libraries(${PROJECT_NAME}.elf lwipmbedtls)

# TODO необходимо написать phy, ethernet
target_link_libraries(${PROJECT_NAME}.elf EthernetLow)

# добавляет внутренние библиотеки
add_subdirectory(${CMAKE_SOURCE_DIR}/IntLibs)
target_link_libraries(${PROJECT_NAME}.elf HalPeripheryLib)

# добавляет дополнительные цели к сборке
include("${CMAKE_SOURCE_DIR}/cmake_Inc/customCommand.cmake")

# добавляет UnitTest
add_subdirectory(${CMAKE_SOURCE_DIR}/UnitTest)

# Add custom target for flashing the program with OpenOCD
add_custom_target(flash
    COMMAND openocd.exe -f interface/atlink.cfg -f target/at32f437xM.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing ${PROJECT_NAME}.elf with OpenOCD"
)
