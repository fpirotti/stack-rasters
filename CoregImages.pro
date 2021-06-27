QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = CoRegisterImages
TEMPLATE = app
VERSION = 2.1.2

MAJOR = 2
MINOR = 1
# VERSION_HEADER = ../version.h
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# versiontarget.target = $$VERSION_HEADER
# versiontarget.commands = ../version/debug/version.exe $$MAJOR $$MINOR $$VERSION_HEADER
# versiontarget.depends = FORCE

PRE_TARGETDEPS += $$VERSION_HEADER
QMAKE_EXTRA_TARGETS += versiontarget


INCLUDEPATH +=  $$quote(D:/OpenCV/opencv/build/include)
#INCLUDEPATH +=  $$quote(D:/OpenCV/openCV-MingW/build/include)
QMAKE_LIBDIR+=     $$quote(D:/OpenCV/opencv/build/x64/vc15/lib)
#QMAKE_LIBDIR+=     $$quote(D:/OpenCV/openCV-MingW/build/x64/mingw/lib)
#QMAKE_LIBS +=   -lopencv_world452

win32 {
    CONFIG(debug, debug|release) {
      QMAKE_LIBS +=   -lopencv_world452d
    #  QMAKE_LIBS +=   -llibopencv_core452.dll.a  -llibopencv_features2d452
    } else {
        QMAKE_LIBS +=   -lopencv_world452
       # QMAKE_LIBS +=   -llibopencv_core452.dll.a  -llibopencv_features2d452
        }
    }


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    worker.cpp

HEADERS += \
    mainwindow.h \
    worker.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
