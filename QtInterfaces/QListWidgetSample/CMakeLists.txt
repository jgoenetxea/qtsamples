cmake_minimum_required(VERSION 2.8)

project(QListWidgetSample)

# Find project dependencies
FIND_PACKAGE( Qt5Widgets REQUIRED )

# Set project files
SET( PROJECT_SOURCES
     main.cpp
     dialogController.cpp
     customListItemWidget.cpp
)

SET( PROJECT_HEADERS
     dialogController.h
     customListItemWidget.h)

SET( PROJECT_HANDLERS
)

# Configure Qt
SET( QT_USE_QTOPENGL TRUE )

SET( PROJECT_FORMS maindialog.ui )

QT5_WRAP_CPP( PROJECT_HEADERS_MOC ${PROJECT_HEADERS} )
QT5_WRAP_UI( PROJECT_FORMS_HEADERS ${PROJECT_FORMS} )
QT5_ADD_RESOURCES( PROJECT_RESOURCES_RCC ${PROJECT_RESOURCES} )
SET( QT_INCLUDE_DIR ${Qt5Widgets_INCLUDE_DIRS} )

SOURCE_GROUP( "Generated Files" FILES ${PROJECT_HEADERS_MOC}
                ${PROJECT_FORMS_HEADERS} ${PROJECT_RESOURCES_RCC} )

# Configure project paths
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}
               ${PROJECT_HANDLERS}
               ${PROJECT_HEADERS}
               ${PROJECT_SOURCES}
               ${PROJECT_HEADERS_MOC}
               ${PROJECT_FORMS_HEADERS}
               ${PROJECT_RESOURCES_RCC}
               )

TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} )

SET( ${PROJECT_NAME}_COMPLETE_SOURCES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)
