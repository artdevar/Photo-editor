import QtQuick 2.0
import QtQuick.Controls 2.5

MainWindow {
    id: root
    title: qsTr("Adjust hue/saturation levels")
    flags: Qt.Dialog | Qt.WindowMinMaxButtonsHint | Qt.WindowTitleHint | Qt.MSWindowsFixedSizeDialogHint
    width: 400
    height: 270

    property bool stateChanged: false

    function makeChanges(hue, sat, light) {
        if (!root.stateChanged) {
            mainWindow.saveState()
            root.stateChanged = true
        }
        mainWindow.actionHueSaturation(hue, sat, light)
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
            text: qsTr("Hue: %1").arg(hueSlider.value)
            color: "white"
            font: mainFont.font
        }
        Slider {
            id: hueSlider
            width: parent.width
            from: 0
            value: 0
            to: 360
            stepSize: 1
            onValueChanged: makeChanges(hueSlider.value, saturationSlider.value, lightnessSlider.value)
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Saturation: %1").arg(saturationSlider.value)
            color: "white"
            font: mainFont.font
        }
        Slider {
            id: saturationSlider
            width: parent.width
            from: -100
            value: 0
            to: 100
            stepSize: 1
            onValueChanged: makeChanges(hueSlider.value, saturationSlider.value, lightnessSlider.value)
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Lightness: %1").arg(lightnessSlider.value)
            color: "white"
            font: mainFont.font
        }
        Slider {
            id: lightnessSlider
            width: parent.width
            from: -100
            value: 0
            to: 100
            stepSize: 1
            onValueChanged: makeChanges(hueSlider.value, saturationSlider.value, lightnessSlider.value)
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