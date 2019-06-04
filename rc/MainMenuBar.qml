import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2

import viewmodels 1.0

MenuBar {
    id: root
    font { family: "SF Pro Text"; pixelSize: 13 }

    property Component brightnessEditorComponent: BrightnessContrastSliders {}
    property Component hueSaturationEditorComponent: HueSaturationSliders {}
    property Component histogramComponent: Histogram {}
    property Component gaussianBlurComponent: GaussianBlurSlider {}

    property bool optionsAvailable: false

    Menu {
        title: qsTr("File")
        font: parent.font

        Action {
            text: qsTr("Open")
            onTriggered: mainWindow.actionOpen()
            //onTriggered: mainWindow.openFile("D:/dev/UNIVER/photo_editor/examples/steep.jpg") // for testing
            enabled: true
        }
        Action {
            text: qsTr("Save")
            onTriggered: mainWindow.actionSave()
            enabled: root.optionsAvailable
        }
        Action {
            text: qsTr("Save As")
            onTriggered: mainWindow.actionSaveAs()
            enabled: root.optionsAvailable
        }
        MenuSeparator {}
        Action {
            text: qsTr("Close")
            onTriggered: mainWindow.actionClose()
            enabled: root.optionsAvailable
        }
    }
    Menu {
        title: qsTr("Edit")
        font: parent.font
        enabled: root.optionsAvailable

        Action {
            text: qsTr("Undo")
            onTriggered: mainWindow.actionUndo()
        }
        Action {
            text: qsTr("Redo")
            onTriggered: console.log("Redo feature has not implemented yet")
        }
        MenuSeparator {}
        Action {
            text: qsTr("Flip vertical")
            onTriggered: mainWindow.actionFlipVertical()
        }
        Action {
            text: qsTr("Flip horizontal")
            onTriggered: mainWindow.actionFlipHorizontal()
        }
        Action {
            text: qsTr("Transpose")
            onTriggered: mainWindow.actionTranspose()
        }
    }
    Menu {
        Loader { id: brightnessEditorLoader }
        Loader { id: hueSaturationEditorLoader }
        Loader { id: histogramLoader }

        title: qsTr("Correction")
        font: parent.font
        enabled: root.optionsAvailable

        Action {
            text: qsTr("Brightness/Contrast")
            onTriggered: {
                brightnessEditorLoader.sourceComponent = brightnessEditorComponent
                brightnessEditorLoader.item.show()
                brightnessEditorLoader.item.closing.connect(function() {brightnessEditorLoader.sourceComponent = undefined})
            }
        }
        Action {
            text: qsTr("Hue/Saturation")
            onTriggered: {
                hueSaturationEditorLoader.sourceComponent = hueSaturationEditorComponent
                hueSaturationEditorLoader.item.show()
                hueSaturationEditorLoader.item.closing.connect(function() {hueSaturationEditorLoader.sourceComponent = undefined})
            }
        }

        Action {
            text: qsTr("Levels")
            onTriggered: {
                histogramLoader.sourceComponent = histogramComponent
                histogramLoader.item.show()
                histogramLoader.item.closing.connect(function() {histogramLoader.sourceComponent = undefined})
            }
        }
        MenuSeparator {}
        Action {
            text: qsTr("Auto contrast")
            onTriggered: mainWindow.actionAutoContrast()
        }
        MenuSeparator {}
        Action {
            text: qsTr("Inversive")
            onTriggered: mainWindow.actionInversion()
        }
        Action {
            text: qsTr("Grayscale")
            onTriggered: mainWindow.actionGrayscale()
        }
        Action {
            text: qsTr("Sepia")
            onTriggered: mainWindow.actionSepia()
        }
    }
    /*Menu {
        title: qsTr("Filter")
        font: parent.font
        enabled: root.optionsAvailable
        Loader { id: gaussianBlurLoader }
        Action {
            text: qsTr("Gaussian blur")
            onTriggered: {
                gaussianBlurLoader.sourceComponent = gaussianBlurComponent
                gaussianBlurLoader.item.show()
                gaussianBlurLoader.item.closing.connect(function() {gaussianBlurLoader.sourceComponent = undefined})
            }
        }
    }*/
}
