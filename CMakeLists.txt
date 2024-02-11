cmake_minimum_required(VERSION 3.5)

project(calibrateTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### 3rdpart
# opencv
set(OpenCV_DIR ${PROJECT_SOURCE_DIR}/3rdpart/opencv-4.8.0/build/install)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
  message(STATUS "OpenCV version: ${OpenCV_VERSION}")
  include_directories(${OpenCV_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "OpenCV not found")
endif()

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
###

# subDir
add_subdirectory(ui)
add_subdirectory(calibrate)
set(UIDIR ${PROJECT_SOURCE_DIR}/ui)
set(CALIDIR ${PROJECT_SOURCE_DIR}/calibrate)

# trans
set(TS_FILES ${UIDIR}/calibrateTool_en_US.ts ${UIDIR}/calibrateTool_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp

        # translate file
        ${TS_FILES}

        # mainwindow
        ${UIDIR}/mainwindow.cpp
        ${UIDIR}/mainwindow.hpp
        ${UIDIR}/mainwindow.ui

        # image And Pattern Properties Dialog
        ${UIDIR}/propertiesdialog.hpp
        ${UIDIR}/propertiesdialog.cpp
        ${UIDIR}/propertiesdialog.ui

        # resource
        src.qrc



        # utils
        ${CALIDIR}/imageFormat.hpp

        # calibrate source file
        ${CALIDIR}/findchessboardcorner.hpp
        ${CALIDIR}/monocalibrate.hpp
        ${CALIDIR}/SteroCalibrate.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calibrateTool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET calibrateTool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(calibrateTool SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(calibrateTool
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(calibrateTool PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS})

set_target_properties(calibrateTool PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS calibrateTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calibrateTool)
endif()
