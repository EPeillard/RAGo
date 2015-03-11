# Ajustez en fonction de votre version de CMake
cmake_minimum_required (VERSION 2.8)

# Nom du projet
project (hello)

find_package (OpenCV REQUIRED)
find_package (aruco REQUIRED)
find_package (Qt4 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH bin)

file(
        GLOB_RECURSE
        source_files
        src/*.cpp
)

file(
        GLOB_RECURSE
        header_files
        src/*.hpp
)

QT4_WRAP_CPP(header_moc ${header_files})

add_executable (hello ${source_files} ${header_moc})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

target_link_libraries (hello ${OpenCV_LIBS} ${aruco_LIBS} ${QT_LIBRARIES})
