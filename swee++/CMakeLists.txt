cmake_minimum_required(VERSION 3.8) # требуется CMake версии >= 3.8
project(swee++ VERSION 0.1)         # проект "swee++" версии 0.1

set(SOURCES                         # файлы реализации
        src/App.cpp
        src/frames/MainFrame.cpp
        src/frames/GameSettingsDialog.cpp)

set(HEADERS                         # заголовочные файлы
        include/wx_pch.h
        include/App.h
        include/ColorScheme.h
        include/frames/MainFrame.h
        include/frames/GameSettingsDialog.h)

#if(UNIX AND NOT APPLE)
#    add_definitions(-D__GXX_ABI_VERSION=1010)
#endif()
if(APPLE)
    add_definitions(-D__GXX_ABI_VERSION=1002)
endif()

include_directories(include)        # каталог заголовочных файлов

# подключение библиотек wxWidgets
find_package(wxWidgets COMPONENTS core base REQUIRED)
include("${wxWidgets_USE_FILE}")

if(WIN32)                           # файл ресурсов для Windows
    set(SOURCES WIN32 ${SOURCES})
    set(HEADERS ${HEADERS} resources.rc)
endif()

# проект - исполняемый файл
add_executable(swee++ ${SOURCES} ${HEADERS})

# заголовочные файлы libswee++
include_directories(${LIBSWEE++_INCLUDE})

# связывание исполняемого файла с библиотеками
if(WIN32)
    target_link_libraries(swee++ libswee++ -static -Wl,-Bstatic ${wxWidgets_LIBRARIES})
else()
    target_link_libraries(swee++ libswee++ ${wxWidgets_LIBRARIES})
endif()

# создание цели сборки "release"
add_custom_target(release ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/__dummy_output")

# сборка готовых пакетов для текущей платформы

if(WIN32)                           # Windows
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__dummy_output"
        DEPENDS swee++
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../assets/win/mkinstaller.bat"
        ARGS "${CMAKE_CURRENT_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
        COMMENT "Building Windows installer...")
endif()

if(UNIX AND NOT APPLE)              # GNU/Linux
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__dummy_output"
        DEPENDS swee++
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../assets/linux/mkpackage.sh"
        ARGS "${CMAKE_CURRENT_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
        COMMENT "Building Debian package...")
endif()

if(APPLE)                           # macOS
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/__dummy_output"
        DEPENDS swee++
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../assets/mac/mkbundle.sh"
        ARGS "${CMAKE_CURRENT_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
        COMMENT "Building macOS app bundle and dmg...")
endif()