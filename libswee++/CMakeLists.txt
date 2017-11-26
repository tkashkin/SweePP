cmake_minimum_required(VERSION 3.8) # требуется CMake версии >= 3.8
project(libswee++ VERSION 0.1)      # проект "libswee++" версии 0.1

set(SOURCES                         # файлы реализации
        src/libswee++.cpp
        src/game/cell.cpp
        src/game/field.cpp)

set(HEADERS                         # заголовочные файлы
        include/libswee++.h
        include/game/cell.h
        include/game/field.h)

add_definitions(-DSWEEPP_EXPORT)

include_directories(include)        # каталог заголовочных файлов

if(WIN32)                           # файл ресурсов для Windows
    set(HEADERS ${HEADERS} resources.rc)
endif()

# проект - разделяемая динамическая библиотека
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

# свойства проекта
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/libswee++.h)

if(UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES SKIP_BUILD_RPATH TRUE)
endif()

# директории заголовочных файлов и файлов реализации
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)