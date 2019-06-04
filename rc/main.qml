import QtQuick 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.5
import QtQuick.Window 2.12

MainWindow {
    id: main
    visible: true
    minimumWidth: 430
    minimumHeight: 200
    width: 1300
    height: 900

    Rectangle {
        id: imageSpace
        width: parent.width
        height: parent.height - menuBar.height - separator.height - (image.visible ? statusBar.height : 0)
        anchors.top: separator.bottom
        color: "#202020"

        Image {
            id: image
            anchors.centerIn: parent
            cache: false
            visible: false
            source: "image://imageProvider/"
            clip: true
            scale: zoomLevel.value

            function reload() {
                var oldSource = source
                source = ""
                source = oldSource
                visible = true
            }

            function remove() {
                visible = false
            }
        }

        DropShadow {
            horizontalOffset: 0
            verticalOffset: 0
            radius: 10
            samples: 20
            color: "black"
            anchors.fill: image
            source: image
            scale: image.scale
            rotation: image.rotation
            visible: image.visible
        }

        Connections {
            target: imageHandler
            onImageSetted: image.reload()
            onImageRemoved: image.remove()
        }

        Connections {
            target: imageCorrector
            onImageEdited: image.reload()
        }
    }

    Rectangle {
        id: menuBar
        height: mainMenu.height
        width: parent.width
        color: "#e0e0e0"

        MainMenuBar {
            id: mainMenu
            anchors.fill: parent
            optionsAvailable: image.visible
        }
    }

    Rectangle {
        id: separator
        width: parent.width
        height: 1
        anchors.top: menuBar.bottom
        color: "#4B4B4B"
    }

    Rectangle {
        id: bottomSeparator
        width: parent.width
        height: separator.height
        anchors.bottom: statusBar.top
        color: separator.color
        visible: statusBar.visible
    }

    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        height: 18
        width: parent.width
        color: "#303030"
        visible: image.visible

        TextInput {
            property real value: {
                var result = text * 0.01
                if (result < 0.05) {
                    result = 0.05
                }
                return result
            }

            id: zoomLevel
            anchors { left: parent.left; leftMargin: 5 }
            width: 25
            height: parent.height
            color: "white"
            font: zoomLevelPercent.font
            text: "100"
            verticalAlignment: TextInput.AlignVCenter
            maximumLength: 3
            validator: RegExpValidator { regExp: /[0-9]{1,3}$/ }
        }

        MainText {
            id: zoomLevelPercent
            anchors.left: zoomLevel.right
            height: parent.height
            text: '%'
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 9
        }

        MainText {
            id: imageSizePxLabel
            anchors.right: parent.right
            anchors.rightMargin: 5
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            font: zoomLevelPercent.font
            text: ("%1x%2 px").arg(image.width).arg(image.height)
        }
    }
}
