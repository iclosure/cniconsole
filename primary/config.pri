################################################################
# CniConsole application
#
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the CniConsole License, Version 1.0
################################################################

######################################################################
# Definitions
######################################################################

######################################################################
# Copyright Information
######################################################################

QMAKE_TARGET_COMPANY = "成都创融达科技有限公司"
QMAKE_TARGET_DESCRIPTION = "CNI System Monitor"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2011-2014 CRDTech CO.,Ltd..All rights reserved."

win32:RC_ICONS = resource/images/global/CniConsole.ico

VER_MAJ = 1
VER_MIN = 2
VER_PAT = 0
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

######################################################################
# kwt dll will be enable or not.
######################################################################

#APP_CONFIG           += kwt_dll

######################################################################
# UiLoader will be enable or not.
######################################################################

#APP_CONFIG           += uitools

######################################################################
# Quick engine will be enable or not.
######################################################################

#APP_CONFIG           += quick

######################################################################
# Install paths
######################################################################

APP_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

unix {
    APP_INSTALL_PREFIX    = /usr/local/$${TARGET}-VERSION
}

win32 {
    APP_INSTALL_PREFIX    = C:/$${TARGET}-VERSION
}

APP_INSTALL_BINS      = $${APP_INSTALL_PREFIX}/bin
APP_INSTALL_DOCS      = $${APP_INSTALL_PREFIX}/doc
APP_INSTALL_HEADERS   = $${APP_INSTALL_PREFIX}/include

