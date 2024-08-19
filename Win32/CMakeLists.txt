cmake_minimum_required(VERSION 3.16)

project(ArmBionicsGUIWin VERSION 0.1 LANGUAGES CXX)

# Enable automatic handling of Qt's MOC, UIC, and RCC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build in release mode (debug mode bugged- too many sections/files too big)
set(CMAKE_BUILD_TYPE Release)

# Find the Qt package
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Additional Qt6 packages
if(${QT_VERSION_MAJOR} EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS SerialPort PrintSupport)
endif()

# Define sources
set(PROJECT_SOURCES
    main.cpp
    emgwidget.cpp
    emgwidget.h
    emgwidget.ui
)

# Add QCustomPlot library
add_subdirectory(QCustomPlot-library)

# Define the executable based on Qt version
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/favicon.rc")
    qt_add_executable(ArmBionicsGUIWin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        definitions.h
        logger.h
        ${app_icon_resource_windows}


    )
else()
    if(ANDROID)
        add_library(ArmBionicsGUIWin SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ArmBionicsGUIWin
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link Qt libraries and custom plot library
if(${QT_VERSION_MAJOR} EQUAL 6)
    target_link_libraries(ArmBionicsGUIWin PRIVATE Qt6::Widgets Qt6::SerialPort Qt6::PrintSupport qcustomplot)
else()
    target_link_libraries(ArmBionicsGUIWin PRIVATE Qt5::Widgets qcustomplot)
endif()

# Compile needed definitions
target_compile_definitions(ArmBionicsGUIWin PRIVATE QCUSTOMPLOT_USE_LIBRARY)

# Set properties for the target
if(${QT_VERSION_MAJOR} EQUAL 5)
    set(BUNDLE_ID_OPTION)
else()
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ArmBionicsGUIWin)
endif()

set_target_properties(ArmBionicsGUIWin PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install rules
include(GNUInstallDirs)
install(TARGETS ArmBionicsGUIWin
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Handle Qt6 executable finalization
if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(ArmBionicsGUIWin)
endif()
