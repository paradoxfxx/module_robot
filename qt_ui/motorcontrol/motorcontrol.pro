#-------------------------------------------------
#
# Project created by QtCreator 2019-12-20T15:01:10
#
#-------------------------------------------------

QT       += core  gui  charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = motorcontrol
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

CONFIG += c++11

SOURCES += \
#        ./../../src/robot_control/robot_joint_client.cpp        \
#        ./../../src/elmo_control/elmo_client.cpp               \
#        ./../../src/ethercat_manager/ethercat_manager.cpp          \
        main.cpp            \
        motorcontrol.cpp    \
        stateplot.cpp       \
        motorThread.cpp

HEADERS += \
#        /usr/xenomai/include/*    \
#        ./../../include/common.h    \
#        ./../../include/robot_control/robot_joint_client.h      \
#        ./../../include/elmo_control/elmo_client.h              \
#        ./../../include/ethercat_manager/ethercat_manager.h     \
        motorcontrol.h \ 
        stateplot.h     \
        motorThread.h

FORMS += \
        motorcontrol.ui \
        stateplot.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
