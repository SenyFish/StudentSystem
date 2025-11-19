cmake_minimum_required(VERSION 3.16)

project(StudentSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 添加 ElaWidgetTools 库
add_subdirectory(ElaWidgetTools/ElaWidgetTools)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        student.cpp
        student.h
        cardwidget.h
        flatcardwidget.cpp
        flatcardwidget.h
        loginwindow.cpp
        loginwindow.h
        user.cpp
        user.h
        usermanager.cpp
        usermanager.h
        operationlog.cpp
        operationlog.h
        resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(StudentSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    # Windows 平台添加应用程序图标
    if(WIN32)
        target_sources(StudentSystem PRIVATE app_icon.rc)
    endif()
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET StudentSystem APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(StudentSystem SHARED
            ${PROJECT_SOURCES}
        )
    # Windows 平台添加应用程序图标
    if(WIN32)
        target_sources(StudentSystem PRIVATE app_icon.rc)
    endif()
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(StudentSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(StudentSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools/ElaWidgetTools
)

target_link_libraries(StudentSystem PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    ElaWidgetTools
)

# 自动复制 ElaWidgetTools.dll 到程序目录
add_custom_command(TARGET StudentSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ElaWidgetTools>
        $<TARGET_FILE_DIR:StudentSystem>
    COMMENT "正在复制 ElaWidgetTools.dll 到程序目录..."
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.StudentSystem)
endif()
set_target_properties(StudentSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE $<IF:$<CONFIG:Debug>,FALSE,TRUE>  # Debug模式显示控制台
)

include(GNUInstallDirs)
install(TARGETS StudentSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(StudentSystem)
endif()
