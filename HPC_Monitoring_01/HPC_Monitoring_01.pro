#-------------------------------------------------
#
# Project created by QtCreator 2019-03-23T12:35:51
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HPC_Monitoring_01
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SYSTEM_NAME = x64         # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64            # '32' or '64', depending on your system

# Define output directories

CONFIG(release, debug|release): CUDA_OBJECTS_DIR = release/cuda
else: CUDA_OBJECTS_DIR = debug/cuda

CUDA_SDK = /usr/local/cuda-10.1
CUDA_DIR = /usr/local/cuda-10.1

CUDA_ARCH = sm_30
NVCCFLAGS = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v --std=c++11 -Xcompiler -fPIE

INCLUDEPATH += $$CUDA_DIR/include
# INCLUDEPATH += /usr/include/
INCLUDEPATH += $$CUDA_DIR/lib64
INCLUDEPATH += $$CUDA_DIR/lib

INCLUDEPATH += /opt/Qt/5.12.2/gcc_64/include/QtGui
INCLUDEPATH += .
INCLUDEPATH += /opt/Qt/5.12.2/gcc_64/include
INCLUDEPATH += /opt/Qt/5.12.2/gcc_64/include/QtWidgets
INCLUDEPATH += /opt/Qt/5.12.2/gcc_64/include/QtCore
INCLUDEPATH += /opt/Qt/5.12.2/gcc_64/include/QtPrintSupport

QMAKE_LIBDIR += $$CUDA_DIR/lib64
QMAKE_LIBDIR += $$CUDA_DIR/lib
QMAKE_LIBDIR += $$CUDA_DIR/include
QMAKE_LIBDIR += $$CUDA_DIR/samples/common/lib

LIBS += -lnvToolsExt
LIBS += -lcuda
LIBS += -lcudart
LIBS += -lGL
LIBS += -lnvidia-ml


CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        qcustomplot.cpp \
    init_nodestatus.cpp \
    init_nodememload.cpp \
    init_nodevgaload.cpp \
    update.cpp \
    update_nodestatus.cpp \
    update_nodememload.cpp \
    update_nodevgaload.cpp \
    launch_noderesume.cpp \
    launch_visualizer.cpp \
    init_visualizer.cpp \
    init_powerestimator.cpp \
    launch_powerestimator.cpp \
    launch_checkpoint.cpp \
    init_checkpoint.cpp \
    init_restart.cpp \
    launch_checkpoint_coordinator.cpp \
    finish_checkpoint_coordinator.cpp \
    launch_restart.cpp \
    finish_checkpoint.cpp \
    finish_restart.cpp


HEADERS += \
        mainwindow.h \
        qcustomplot.h


FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(cuda.pri)

RESOURCES += \
    resource.qrc
