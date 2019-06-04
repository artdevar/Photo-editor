import QtQuick 2.0
import QtQuick.Controls 2.5

MainWindow {
    id: root
    title: qsTr("Adjust brightness/contrast levels")
    flags: Qt.Dialog | Qt.WindowMinMaxButtonsHint | Qt.WindowTitleHint | Qt.MSWindowsFixedSizeDialogHint
    width: 400
    height: 200

    property bool stateChanged: false

    function makeChanges(brightness, contrast) {
        if (!root.stateChanged) {
            mainWindow.saveState()
            root.stateChanged = true
        }
        mainWindow.actionBrightnessContrast(brightness, contrast)
    }

    MainText {
        id: mainFont
        font.pointSize: 11
    }

    Column {
        anchors {
            fill: parent
            topMargin: 10
        }
        spacing: 2
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Brightness: %1").arg(brightnessSlider.value)
            color: "white"
            font: mainFont.font
        }
        Slider {
            id: brightnessSlider
            width: parent.width
            from: -255
            value: 0
            to: 255
            stepSize: 1
            onValueChanged: makeChanges(brightnessSlider.value, contrastSlider.value)
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Contrast: %1").arg(contrastSlider.value)
            color: "white"
            font: mainFont.font
        }
        Slider {
            id: contrastSlider
            width: parent.width
            value: 1.0
            from: 0.0
            to: 15.0
            stepSize: 0.05
            onValueChanged: makeChanges(brightnessSlider.value, contrastSlider.value)
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30
            Button {
                flat: true
                text: qsTr("OK")
                font: mainFont.font
                onClicked: {
                    if (root.stateChanged) {
                        mainWindow.applyChanges()
                    }
                    root.close()
                }
            }
            Button {
                flat: true
                text: qsTr("CANCEL")
                font: mainFont.font
                onClicked: {
                    if (root.stateChanged) {
                        mainWindow.actionUndo()
                    }
                    root.close()
                }
            }
        }
    }
}