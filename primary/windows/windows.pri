################################################################
# cniconsole application
#
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

contains(APP_CONFIG, kwt_dll) {
    DEFINES += Q_WS_WIN KWT_DLL
}

contains(APP_CONFIG, uitools) {
    QT += uitools
}

contains(APP_CONFIG, quick) {
    QT += declarative quick
}

################################################################
# import sources in subdirectory
################################################################

################################################################
# import sources in current directory
################################################################

HEADERS += \
    windows/main_widget.h \
    windows/title_widgets/mainmenu_widgets/main_menu.h \
    windows/title_widgets/mainmenu_widgets/about_dialog.h \
    windows/title_widgets/mainmenu_widgets/settings/setting_dialog.h \
    windows/title_widgets/mainmenu_widgets/settings/network_widget.h \
    windows/title_widgets/mainmenu_widgets/settings/serialport_widget.h

SOURCES += \
    windows/main.cpp \
    windows/main_widget.cpp \
    windows/title_widgets/mainmenu_widgets/main_menu.cpp \
    windows/title_widgets/mainmenu_widgets/about_dialog.cpp \
    windows/title_widgets/mainmenu_widgets/settings/setting_dialog.cpp \
    windows/title_widgets/mainmenu_widgets/settings/network_widget.cpp \
    windows/title_widgets/mainmenu_widgets/settings/serialport_widget.cpp

################################################################
# import target sources in current directory
################################################################

HEADERS += \
    windows/cniconsole/version.h \
    windows/cniconsole/cni_item_widget.h \
    windows/cniconsole/client_widget_proxy.h \
    windows/cniconsole/output_widget.h \
    windows/cniconsole/toolbar_proxy.h \
    windows/cniconsole/statusbar_proxy.h \
    windows/cniconsole/module_widget_proxy.h \
    windows/cniconsole/database/database_manager.h \
    windows/cniconsole/database/cni_xml_parse.h \
    windows/cniconsole/modules/altimeter_widget.h \
    windows/cniconsole/modules/tacanes_widget.h \
    windows/cniconsole/modules/tacanes_advance_widget.h \
    windows/cniconsole/modules/tacanss_widget.h \
    windows/cniconsole/modules/tacanss_advance_widget.h \
    windows/cniconsole/modules/mwpdmd_widget.h \
    windows/cniconsole/modules/mwpdmd_advance_widget.h \
    windows/cniconsole/modules/instrument_widget.h \
    windows/cniconsole/modules/instrument_advance_widget.h \
    windows/cniconsole/protocol/protocol.h \
    windows/cniconsole/protocol/network_proxy.h \
    windows/cniconsole/protocol/serialport_proxy.h

SOURCES += \
    windows/cniconsole/cni_item_widget.cpp \
    windows/cniconsole/client_widget_proxy.cpp \
    windows/cniconsole/output_widget.cpp \
    windows/cniconsole/toolbar_proxy.cpp \
    windows/cniconsole/statusbar_proxy.cpp \
    windows/cniconsole/database/database_manager.cpp \
    windows/cniconsole/database/cni_xml_parse.cpp \
    windows/cniconsole/module_widget_proxy.cpp \
    windows/cniconsole/modules/altimeter_widget.cpp \
    windows/cniconsole/modules/tacanes_widget.cpp \
    windows/cniconsole/modules/tacanes_advance_widget.cpp \
    windows/cniconsole/modules/tacanss_widget.cpp \
    windows/cniconsole/modules/tacanss_advance_widget.cpp \
    windows/cniconsole/modules/mwpdmd_widget.cpp \
    windows/cniconsole/modules/mwpdmd_advance_widget.cpp \
    windows/cniconsole/modules/instrument_widget.cpp \
    windows/cniconsole/modules/instrument_advance_widget.cpp \
    windows/cniconsole/protocol/network_proxy.cpp \
    windows/cniconsole/protocol/serialport_proxy.cpp

################################################################
# import simulate sources
################################################################

CONFIG(debug, debug|release): {

    DEFINES += CNI_SIMULATE

    HEADERS += \
        windows/cniconsole/simulate/network_simulate.h \
        windows/cniconsole/simulate/serialport_simulate.h

    SOURCES += \

}
