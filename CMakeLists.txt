# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)

# Nom du projet
project (hello)

find_package (OpenCV REQUIRED)
find_package (aruco REQUIRED)

set(EXECUTABLE_OUTPUT_PATH bin)

file(
        GLOB_RECURSE
        source_files
        src/*
)


add_executable (hello ${source_files})

target_link_libraries (hello ${OpenCV_LIBS} ${aruco_LIBS})
