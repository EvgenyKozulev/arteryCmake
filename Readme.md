# **Artery cmake project**

Данный проект был разработан с целью обновления проектов mzta под сборку через cmake c использованием элементов кросскомпиляции.

## *Кросскомпиляция*

### Поддерживаемые устройства

MCU Artery AT32F437xM

### Планируется поддерживать устройства

MCU nxp MKVF1M0VLL24

## *Внешние библиотеки*

### Поддерживаемые библиотеки

CMSIS+HAL Artery    
freeRTOS

### Планируется поддерживать библиотеки

lwip    
CodeSys

[//]: # (необходимо переделать с использованием графсхемы)

## *Структура проекта*

```
├── CMakeLists.txt                      // Главный cmake list
|
|
├── ArteryPlatform                      // настройка платформы
|   ├── inc                             // библиотеки настройки тактирования - autogen
|   ├── linker                          // !!! видоизмененный скрипт линкера (определение начала и конца динамической памяти)
|   ├── src                             // src библиотек тактирования - autogen
|   ├── clockGen.APCC                   // autogen AT32_New_Clock_Configuration.exe 
|   └── CMakeLists.txt                  // cmake для сборки платформы+тактирование
|
├── ExternLibOpts                       // Настройки внешних библиотек
│   ├── FreeRtosConfig                  // Настройки FreeRTOS
|   ├── lwipSetting                     // Настройки и опции lwip
|   |   ├── port+phy+mdio               // arteryEmac+arteryPHY+port
|   |   └── CMakelists.txt              // запускает родной toolchain от lwip связывает lwip c artery emac через EthernetArtery
|   └── VisibilityOPTS.cmake            // настраивает видимость headers библиотек и настроек в проекте
|
├── githublibs                          // библиотеки подключенные в виде субмодулей с https://github.com 
|   ├── FreeRTOS                        // submodule FreeRTOS https://github.com/FreeRTOS/FreeRTOS
|   |   ├── CMakeLists.txt              // Родной cmakeList FreeRTOS
|   |   └── /.FreeRTOS./
|   |   
|   ├── lwip                            // submodule lwip https://git.savannah.nongnu.org/git/lwip.git
|   |   ├── src
|   |   |   └── Filelists.cmake         // Родной toolchain от lwip      
|   |   └── /.LWIP./
|   |
|   └── AT32F435_437_Firmware_Library   // submodule https://github.com/ArteryTek/AT32F435_437_Firmware_Library.git 
|
|
|
|
├── NewLibs                             // Нет библиотек для Artery от производителя - необходимы для arm-none-eabi-gcc
|   └── /.NewLib./
|
├── main                                // main
│   ├── main.h
│   └── main.c
|
├── IntLibs                             // Внутренние библиотеки (на данный момент модуль не реализован)
│   ├── CMakeLists.txt                  // CMakeLists для сборки всех внутренних библиотек 
|   ├── lib_1                           // Пример библиотеки
|   |   ├── CMakeLists.txt              // CMakeLists опционально если необходима Кроссплатформенность
|   |   ├── /.head./                    
|   |   └── /.src./                     
|   └──/.libs./
|
├── UnitTest                            // Автоматически собирает проект с тестом
|   ├── CMakeLists.txt                  // Собирает .elf из всех папок тестирования.
│   └── /.Tests./
└── README.md
```

## *Среда разработки*

Технология сборки cmake не предусматривает конкретной среды\IDE при наличии инструментов cmake установленного на ПК сборка может осуществляться любой IDE.
Главный cmakelists предусматривает наличие (установленного и настроенного на ПК) компилятора (arm-none-eabi-gcc) и сопутствующего программного обеспечения к нему.
Автор при подготовке данного проекта использовал:
- win10
- vsCode
    - расширение vsCode: ms-vscode.cpptools
    - расширение vsCode: ms-vscode.cpptools-extension-pack
    - расширение vsCode: twxs.cmake
    - расширение vsCode: ms-vscode.cmake-tools
    - расширение vsCode: marus25.cortex-debug
    - расширение vsCode: MS-CEINTL.vscode-language-pack-ru

- пакеты msys2:
    - pacman -S mingw-w64-ucrt-x86_64-arm-none-eabi-toolchain
    - pacman -S mingw-w64-ucrt-x86_64-cmake
    - pacman -S mingw-w64-ucrt-x86_64-ninja
    - pacman -S mingw-w64-ucrt-x86_64-gdb-multiarch
    - pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain // опционально не влияет на проект

[vsCode](https://code.visualstudio.com/)
Visual Studio Code — это легкий, но мощный редактор исходного кода, который работает на вашем рабочем столе и доступен для Windows, macOS и Linux. Он поставляется со встроенной поддержкой JavaScript, TypeScript и Node.js и имеет богатую экосистему расширений для других языков и сред выполнения (таких как C++, C#, Java, Python, PHP, Go, .NET).

Если Вы впервый раз будете использовать данную IDE рекомендуется ознакомиться с [документацией](https://code.visualstudio.com/docs)
При создании проекта Автор руководствовался [инструкцией](https://code.visualstudio.com/docs/cpp/config-mingw) предоставленной разработчиками vsCode, для понимания механизма сборки Автор настоятельно рекомендует прочитать и выполнить все пункты пошаговой инструкции.

[MSYS2](https://www.msys2.org/)
 — это набор инструментов и библиотек, предоставляющий вам простую в использовании среду для создания, установки и запуска собственного программного обеспечения Windows.

Если Вы никогда не использовали msys2 выполните пошаговую [инструкцию](https://www.msys2.org/) предоставленную производителем.

### Настройка проекта vsCode+msys2

Склонируйте проект: 

    git clone ssh://git@bitbucket.mzta.ru:7999/emb/arterycmakeproject.git --recurse-submodules

Зайдите в данный репозиторий локально на ПК с помощью IDE. 

После установки vsCode и набора инструментов msys2 последовательно выполните установку необходимых пакетов для разработки:

пакет инструментов arm-none-eabi:

    pacman -S mingw-w64-ucrt-x86_64-arm-none-eabi-toolchain

пакет инструментов cmake:

    pacman -S mingw-w64-ucrt-x86_64-cmake   

пакет инструментов кодогенератора:

    pacman -S mingw-w64-ucrt-x86_64-ninja   

пакет инструментов отладчика (не рекомендуется использовать arm-none-eabi-gdb возможен конфликт с Python):

    pacman -S mingw-w64-ucrt-x86_64-gdb-multiarch   

Обновите установленные пакеты командой: pacman -Suy

        kozulev@NTC-42 MSYS ~
        $ pacman -Suy
        :: Synchronizing package databases...
        clangarm64 is up to date
        mingw32 is up to date
        mingw64 is up to date
        ucrt64 is up to date
        clang32 is up to date
        clang64 is up to date
        msys is up to date
        :: Starting core system upgrade...
        there is nothing to do
        :: Starting full system upgrade...
        resolving dependencies...
        looking for conflicting packages...
        Packages (65)   binutils-2.41-3  bison-3.8.2-5  brotli-1.1.0-1    
                        mingw-w64-ucrt-x86_64-zlib-1.3-1  mpfr-4.2.1-1  
                        ncurses-6.4-2  openssl-3.1.3-1  pkgconf-2.0.2-2  
                        tar-1.35-2  xz-5.4.4-1  zlib-1.3-1
        Total Download Size:    159.85 MiB
        Total Installed Size:  1197.08 MiB
        Net Upgrade Size:        59.25 MiB
        :: Proceed with installation? [Y/n] y

Данную команду(pacman -Suy) необходимо повторять до получения последней версии, Вы получите сообщение:

        kozulev@NTC-42 MSYS ~
        $ pacman -Suy
        :: Synchronizing package databases...
        clangarm64 is up to date
        mingw32 is up to date
        mingw64 is up to date
        ucrt64 is up to date
        clang32 is up to date
        clang64 is up to date
        msys is up to date
        :: Starting core system upgrade...
        there is nothing to do
        :: Starting full system upgrade...
        there is nothing to do

Данное сообщение говорит о том, что инструменты обновлены до последней версии.

Для корректной работы установленных инструментов Вам необходимо добавить адрес установки в переменные среды пользователя. 
Вы можете это выполнить:
1. пуск->параметры->Дополнительные параметры системы->Переменные среды 
            в меню выбрать переменные среды для пользователя раздел path и добавить путь к установленным инструментам msys2
            например у Автора получился такой путь: c:\set\ucrt64\bin\ потому что Автор устанавливал msys2 по адресу: c:\set.
2. средствами командной строки выполняя [инструкцию](https://learn.microsoft.com/ru-ru/sql/integration-services/lesson-1-1-creating-working-folders-and-environment-variables?view=sql-server-ver16)
3. другими способами...

Выполните настройку проекта через cmake возможны варианты:
1. с помощью расширения vsCode: ms-vscode.cmake-tools.
2. с помощью командной строки [инструкция](https://cmake.org/cmake/help/latest/command/build_command.html)
3. другими способами... 

### Настройка загрузчика

Пошаговая инструкция:
1. Проверьте поддержку Вашего устройства с помощью [openocd](https://openocd.org/).
Или уточните у производителя (например: для микроконтроллеров Artery производитель поставляет собственную версию openocd использовать только её)
2. Добавьте путь к openocd.exe в раздел path переменные среды пользователя.
3. Измените в главном файле CMakeLists настройки openocd для Вашего устройства (-f interface/atlink.cfg -f target/at32f437xM.cfg )
3. Выполните target flash через cmake.

### Настройка GDB

Настройку отладчика GDB можно выполнить следующими способами:
1. Через настройку Launch.json средствами vsCode (У Автора данный способ не заработал).
2. Через расширение vsCode: marus25.cortex-debug.

#### vsCode: marus25.cortex-debug

[marus25.cortex-debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) - расширение для vsCode предназначенное для отладки.
Подробная инструкция для настройки имеется на сайте производителя. Автор прилагает свой вариант настройки файла launch.json:

        {
            "version": "0.2.0",
            "configurations": [
                {
                    "name": "Cortex Debug",
                    "cwd": "${workspaceFolder}",
                    "executable": "${workspaceFolder}/build/Artery.elf",    // Artery  название проекта
                    "request": "launch",
                    "type": "cortex-debug",
                    "runToEntryPoint": "main",
                    "servertype": "openocd",
                    "searchDir": [
                        "c:/Program Files/AT32IDE/OpenOCD/scripts/"         // путь к openocd scripts
                    ],
                    "device": "AT437ZMT7",                                  // наименование устройства
                    "showDevDebugOutput": "raw",                            // для вывода дополнительной отладки в терминал
                    "configFiles": [
                        "interface/atlink.cfg",                             // опции программатора
                        "target/at32f437xM.cfg"                             // опции MCU
                    ],
                    "preLaunchCommands": [
                        "set mem inaccessible-by-default off"
                    ],
                    "postLaunchCommands": [
                        "monitor reset run",
                        "monitor sleep 200",
                        "monitor halt"
                    ],
                }
            ]
        }

## IntelliSense

[IntelliSense](https://code.visualstudio.com/docs/editor/intellisense) - это инструмент, который поможет Вам писать код быстрее и эффективнее за счет добавления функций редактирования кода, таких как завершение кода, информация о параметрах, краткая информация и списки участников.
Для настройки IntelliSense Автор предлагает настроить файл c_cpp_properties.json следующим образом:

    {
        "configurations": [
            {
                "name": "arm-none-eabi",
                "includePath": [
                    "${workspaceFolder}/**"
                ],
                "defines": [
                    "AT32F437ZMT7"
                ],
                "compilerPath": "C:/set/ucrt64/bin/arm-none-eabi-gcc.exe",
                "cStandard": "c11", 
                "cppStandard": "c++11",
                "intelliSenseMode": "gcc-x64",
                "configurationProvider": "ms-vscode.cmake-tools",
                "compilerArgs": [ 
                    "-g",
                    "-Os",
                    "-Wall",
                    "-Wextra",
                    "-pipe",
                    "-Wshadow",
                    "-Wpointer-arith",
                    "-Wbad-function-cast",
                    //"-Wcast-align",
                    "-Wsign-compare",
                    "-Waggregate-return",
                    "-Wstrict-prototypes",
                    "-Wmissing-prototypes",
                    "-Wmissing-declarations",
                    "-Wunused"
                ]
            }
        ],
        "version": 4
    }


## Bugs

- отладчик не корректно запускает точки остановки необходимо сделать рестарт 

