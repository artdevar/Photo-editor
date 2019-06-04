import QtQuick 2.0
import QtQuick.Controls 2.5
import viewmodels 1.0

MainWindow {
    id: root
    title: qsTr("Levels")
    flags: Qt.Tool
    width: colorHistogramRectangle.width + 10
    height: colorHistogramRectangle.height + channelRow.height + ranges.height

    MainText {
        id: mainFont
        font.pixelSize: 15
    }

    Row {
        id: channelRow
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: 50
        spacing: 5
        MainText {
            id: channelText
            font.pixelSize: 15
            anchors.verticalCenter: parent.verticalCenter
            text: "Channel:"
        }
        Tumbler {
            anchors.verticalCenter: parent.verticalCenter
            height: 125
            spacing: 1
            font: mainFont.font
            model: ["RGB", "Red", "Green", "Blue"]
            onCurrentIndexChanged: colorHistogram.currentChannel = currentIndex
        }
    }

    Rectangle {
        id: colorHistogramRectangle
        width: colorHistogram.histogramWidth
        height: colorHistogram.histogramHeight
        anchors { top: channelRow.bottom; horizontalCenter: parent.horizontalCenter}
        color: "#181818"

        ColorHistogram {
            id: colorHistogram
            anchors.fill: parent
            onChannelChanged: colorHistogram.update()
        }

        border { color: "black"; width: 1 }
    }

    Item {
        id: ranges
        anchors {
            top: colorHistogramRectangle.bottom
            left: colorHistogramRectangle.left
        }
        width: colorHistogramRectangle.width
        height: 12
        Repeater {
            model: [0, 64, 128, 192, 255]
            MainText {
                color: "white"
                text: modelData
                x: modelData * 2 - index * 4.3
            }
        }
    }
}
