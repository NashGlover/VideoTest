#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T15:41:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    VideoStreamer.cpp

HEADERS  += mainwindow.h \
    VideoStreamer.h

FORMS    += mainwindow.ui

macx {
	QT_CONFIG -= no-pkg-config
	QMAKE_CXXFLAGS += -m32
	QMAKE_CXXFLAGS += -isystem /opt/local/include
}

unix {
	CONFIG += link_pkgconfig
	PKGCONFIG += opencv
}

win32 {
	INCLUDEPATH += C:/opencv/build/include
	LIBS += -LC:/opencv/build/x86/mingw/bin
	LIBS += -lopencv_core248d -lopencv_highgui248d -lopencv_imgproc248d
}

