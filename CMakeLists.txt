cmake_minimum_required(VERSION 3.8) # требуется CMake версии >= 3.8
project("Swee++")                   # проект "Swee++"

set(CMAKE_CXX_STANDARD 17)          # использовать стандарт C++17

# путь к заголовочным файлам libswee++
set(LIBSWEE++_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/libswee++/include")

add_subdirectory("libswee++")       # добавить подпроект "libswee++"
add_subdirectory("swee++")          # добавить подпроект "swee++"