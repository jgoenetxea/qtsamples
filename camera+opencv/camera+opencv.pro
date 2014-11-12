TEMPLATE = app
TARGET = camera

QT += multimedia multimediawidgets

HEADERS = \
    camera.h \
    imagesettings.h \
    videosettings.h \
    ViulibCamera.h

SOURCES = \
    main.cpp \
    camera.cpp \
    imagesettings.cpp \
    videosettings.cpp \
    ViulibCamera.cpp

FORMS += \
    camera.ui \
    videosettings.ui \
    imagesettings.ui

target.path = camera
INSTALLS += target

QT+=widgets
