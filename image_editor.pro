QT     += qml quick quickcontrols2 widgets core5compat
CONFIG += c++17 resources_big object_parallel_to_source qmltypes

QML_IMPORT_NAME = Custom
QML_IMPORT_MAJOR_VERSION = 1

TARGET = image_editor

INCLUDEPATH += "includes"
INCLUDEPATH += "widgets"

SOURCES +=  src/ColorHistogram.cpp \
            src/Commands.cpp \
            src/CommandsHistory.cpp \
            src/Converter.cpp \
            src/Editor.cpp \
            src/HistogramProvider.cpp \
            src/ImageProvider.cpp \
            src/main.cpp \
            widgets/FileDialog.cpp

HEADERS +=  \
            includes/ColorHistogram.h \
            includes/Commands.h \
            includes/CommandsHistory.h \
            includes/Converter.h \
            includes/Editor.h \
            includes/HistogramProvider.h \
            includes/ImageProvider.h \
            widgets/FileDialog.h

RESOURCES += rc/qml.qrc
