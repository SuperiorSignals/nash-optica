#-------------------------------------------------
#
# Project created by QtCreator 2018-05-23T08:43:17
#
#-------------------------------------------------

QT       += core gui opengl
LIBS    += -lopengl32 -glu32 #-lglut32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = optics_sample_7_qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lenswindow.cpp \
    customlens.cpp \
    intensityprofile.cpp \
    lens.cpp \
    matrix.cpp \
    specification.cpp \
    sphere.cpp \
    sphericallens.cpp

HEADERS  += mainwindow.h \
    lenswindow.h \
    customlens.h \
    intensityprofile.h \
    lens.h \
    matrix.h \
    position.h \
    ray.h \
    specification.h \
    sphere.h \
    sphericallens.h

FORMS    += mainwindow.ui
