QT += qml
QT += quick
QT += quickcontrols2
QT += widgets
CONFIG += c++17
CONFIG += resources_big

TARGET = photo_editor

INCLUDEPATH += "includes"
INCLUDEPATH += "widgets"

SOURCES += $$PWD/src/*.cpp \
           $$PWD/widgets/*.cpp

HEADERS += $$PWD/includes/*.h \
           $$PWD/widgets/*.h

RESOURCES += rc/qml.qrc