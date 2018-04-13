cmake_minimum_required(VERSION 2.8.12)

project(QtCursorManagement)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp"
                               "qtClient.cpp"
                               "qtClient.h"
                               "qcvMatViewer.cpp"
                               "qcvMatViewer.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
