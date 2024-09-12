cmake_minimum_required(VERSION 3.5)
project(GTKapp)

# Установка стандартов C и C++
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Установка переменных для путей GTK4
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

# Найти GTK4 с помощью pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtkmm-4.0)

# Добавление директорий для включаемых файлов и линковки
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

# Добавление дефиниций и линковка библиотек GTK4
add_definitions(${GTK4_CFLAGS_OTHER})

# Создание исполняемого файла
add_executable(App main.cpp)

# Установка свойств для исполняемого файла (например, чтобы не показывать консольное окно)
set_target_properties(App PROPERTIES WIN32_EXECUTABLE true)

# Линковка с библиотеками GTK4
target_link_libraries(App ${GTK4_LIBRARIES})
