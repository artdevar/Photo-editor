import QtQuick 2.0
import QtQuick.Controls 2.5

MainWindow {
    id: root
    title: qsTr("Gaussian blur")
    flags: Qt.Dialog | Qt.WindowMinMaxButtonsHint | Qt.WindowTitleHint | Qt.MSWindowsFixedSizeDialogHint
    width: 400
    height: 130

    property bool stateChanged: false
    readonly property int startValue: 50

    function makeChanges(value) {
        if (!root.stateChanged) {
            mainWindow.saveState()
            root.stateChanged = true
        }
        mainWindow.actionGaussianBlur(value)
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
            text: qsTr("Blur: %1").arg(blurSlider.value)
            color: "white"
            font: mainFont.font
        }
        Slider {
            id: blurSlider
            width: parent.width
            from: 10
            value: root.startValue
            to: 250
            stepSize: 1
            onMoved: root.makeChanges(value)
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