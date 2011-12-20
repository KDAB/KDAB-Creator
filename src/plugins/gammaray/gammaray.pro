TEMPLATE = lib
TARGET = Gammaray

# DEFINES += 

PROVIDER = KDAB

include(../../qtcreatorplugin.pri)
include(gammaray_dependencies.pri)

LIBS += -L$$IDE_PLUGIN_PATH/Nokia

# QT += 

# Gammaray files

SOURCES += \
    gammarayplugin.cpp \
    gammaraytool.cpp \
    gammarayengine.cpp \
    gammarayruncontrolfactory.cpp \
    gammaraysettings.cpp \
    gammarayconfigwidget.cpp

HEADERS += \
    gammarayplugin.h \
    gammaraytool.h \
    gammarayengine.h \
    gammarayconstants.h \
    gammarayruncontrolfactory.h \
    gammaraysettings.h \
    gammarayconfigwidget.h

FORMS += \
    gammarayconfigwidget.ui \
    gammarayversion.ui

#RESOURCES += \
