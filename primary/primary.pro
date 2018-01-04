################################################################
# Project created by QtCreator 2013-06-04T13:26:00
#
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft, version 1.0
################################################################

include( config.pri )
include( windows/windows.pri )

################################################################
# project config
################################################################

QT += core gui widgets network declarative

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += winextras serialport qml quick
}

TEMPLATE = app
TARGET = CniConsole
DESTDIR = $${PWD}/../output/bin

######################################################################
# qmake internal options
######################################################################

#CONFIG += thread static
#CONFIG += static
CONFIG -= app_bundle

######################################################################
# release/debug mode
######################################################################

win32 {
    CONFIG(release, debug|release): {
        DEFINES += QT_NO_DEBUG_OUTPUT
    }
} else {
    CONFIG += release
    DEFINES += QT_NO_DEBUG_OUTPUT
}

linux-g++ {
    CONFIG += separate_debug_info
}

################################################################
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

CONFIG += precompile_header
PRECOMPILED_HEADER = precomp.h
INCLUDEPATH += .

################################################################
# Resouces
################################################################

RESOURCES += \
    resource/images/primary_images.qrc \
    resource/lang/primary_lang.qrc \
#    resource/qml/primary_qml.qrc \
    resource/qss/primary_qss.qrc

################################################################
# Translations
################################################################

TRANSLATIONS += \
    resource/lang/zh_CN.ts \
    resource/lang/en_US.ts

################################################################
# Others
################################################################

OTHER_FILES += \
    ../output/data/cni_data_defaults.xml \
    ../output/resource/mainframe/ButtonQuit.qml \
    ../output/resource/mainframe/PageSelection.qml \
    ../output/resource/mainframe/ButtonArrow.qml \
    ../output/resource/mainframe/PageProxy.qml \
    ../output/resource/MainFrame.qml \
    ../output/resource/Page1.qml \
    ../output/resource/Page2.qml \
    ../output/resource/Page3.qml \
    ../output/resource/Page4.qml \
    ../output/resource/Page5.qml \
    ../output/resource/Page6.qml

################################################################
# import libraries
################################################################

## import Kwt library

win32:CONFIG(release, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/Kwt-3.1.0/lib/ -lQt$${QT_MAJOR_VERSION}Kwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/Kwt-3.1.0/lib/ -lQt$${QT_MAJOR_VERSION}Kwtd
else:unix: LIBS += -L$$(SMARTKITS_HOME)/Qt/Kwt-3.1.0/lib/ -lQt$${QT_MAJOR_VERSION}Kwt

INCLUDEPATH += $$(SMARTKITS_HOME)/Qt/Kwt-3.1.0/include
DEPENDPATH += $$(SMARTKITS_HOME)/Qt/Kwt-3.1.0/include

## import tinyxml library

win32:CONFIG(release, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/tinyxml-2.6.2/lib/ -ltinyxml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/tinyxml-2.6.2/lib/ -ltinyxmld
else:unix: LIBS += -L$$(SMARTKITS_HOME)/Qt/tinyxml-2.6.2/lib/ -ltinyxml

INCLUDEPATH += $$(SMARTKITS_HOME)/Qt/tinyxml-2.6.2/include
DEPENDPATH += $$(SMARTKITS_HOME)/Qt/tinyxml-2.6.2/include
