cmake_minimum_required(VERSION 3.15)
project(UnitTest VERSION 0.1.0 LANGUAGES C ASM)

file(GLOB_RECURSE SrcTest "./*.c")
message("unit test::${SrcTest}")

set(NewLib "${CMAKE_SOURCE_DIR}/NewLibs/newLibs.c")

set(count 1)

foreach(X IN LISTS SrcTest)
    set(UnitTest "test${count}")
    add_executable(${UnitTest}.elf ${X} ${NewLib})
    target_link_libraries(${UnitTest}.elf ArteryPlatform)
    target_link_libraries(${UnitTest}.elf freertos_kernel)
    target_link_libraries(${UnitTest}.elf EthernetArtery)
    MATH(EXPR count "${count}+1")
endforeach()
