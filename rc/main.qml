import QtQuick
import Qt5Compat.GraphicalEffects
import QtQuick.Window

MainWindow {
  id: root
  visible: true
  minimumWidth: 430
  minimumHeight: 200
  width: 1300
  height: 900
  onClosing: function(close) {
    close.accepted = editor.isAllowedToCloseEditor()
  }

  Rectangle {
    id: imageSpace

    property real absoluteHeight: parent.height - menuBar.height - separator.height - statusBar.height

    width: parent.width
    height: absoluteHeight + (image.visible ? 0 : statusBar.height)
    anchors.top: separator.bottom
    color: "#d9d9d9"

    MouseArea {
      id: imageSpaceMouseArea
      anchors.fill: imageSpace
      onWheel: function(wheel) {
        zoomLevel.onWheelScroll(wheel.angleDelta.y)
      }
    }

    Image {
      id: image
      anchors.centerIn: imageSpace
      cache: false
      asynchronous: true
      retainWhileLoading: true
      smooth: false
      visible: false
      source: "image://provider"
      clip: true
      scale: zoomLevel.value

      Behavior on scale {
        enabled: image.visible
        PropertyAnimation {
          easing.type: Easing.Linear
          duration: 50
        }
      }

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
      color: "#212121"
      anchors.fill: image
      source: image
      scale: image.scale
      rotation: image.rotation
      visible: image.visible
    }

    Connections {
      target: editor

      function onCommandExecuted(commandDescription) {
        if (commandDescription !== "")
          actionDescription.showAction(commandDescription)
      }
    }

    Connections {
      target: imageProvider

      function onImageOpened(width, height) {
        image.reload()

        var scaleWidth  = imageSpace.width  / width
        var scaleHeight = imageSpace.absoluteHeight / height
        zoomLevel.value = Math.min(1.0, Math.min(scaleWidth, scaleHeight) - 0.01)
      }

      function onImageEdited() {
        image.reload()
      }

      function onImageClosed() {
        image.remove()
      }

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
    color: "white"
    visible: image.visible
    clip: true

    TextInput {
      id: zoomLevel

      property real value: 1.0

      anchors {
        left: parent.left;
        leftMargin: 5
      }
      width: 25
      height: parent.height
      color: "black"
      font: zoomLevelPercent.font
      verticalAlignment: TextInput.AlignVCenter
      text: valueToText()

      validator: IntValidator {
        bottom: 1
        top: 999
      }

      onAccepted: {
        zoomLevel.focus = false
        value = valueFromText()
      }

      function valueToText() {
        return Math.round(value * 100).toString()
      }

      function valueFromText() {
        var num = parseInt(text)
        if (isNaN(num) || num == 0)
          num = 1
        num = num * 0.01

        return num
      }

      function onWheelScroll(delta) {
        var zoomDelta = 0.03
        if (delta > 0)
          value += zoomDelta
        else
          value -= zoomDelta

        value = Math.max(0.01, value)
      }
    }

    MainText {
      id: zoomLevelPercent
      anchors {
        left: zoomLevel.right
        verticalCenter: statusBar.verticalCenter
      }
      text: "%"
      color: "black"
    }

    MainText {
      id: actionDescription
      anchors {
        verticalCenter: statusBar.verticalCenter
        horizontalCenter: statusBar.horizontalCenter
      }
      color: "black"
      text: nextActionText

      property int transitionDuration: 200
      property string nextActionText: ""

      state: "hidden"
      states: [
        State { name: "shown"  },
        State { name: "hidden" }
      ]

      transitions: [
        Transition {
          from: "hidden"; to: "shown"
          PropertyAnimation {
            target: actionDescription
            property: "anchors.verticalCenterOffset"
            from: 20; to: 0
            easing.type: Easing.OutBack
            duration: actionDescription.transitionDuration
          }
        },
        Transition {
          from: "shown"; to: "hidden"
          PropertyAnimation {
            target: actionDescription
            property: "anchors.verticalCenterOffset"
            from: 0; to: -20
            easing.type: Easing.InBack
            duration: actionDescription.transitionDuration
          }
        }
      ]

      Timer {
        id: hideDescriptionTimer
        interval: 3000 + actionDescription.transitionDuration
        onTriggered: actionDescription.state = "hidden"
      }

      Timer {
        id: showDescriptionTimer
        interval: actionDescription.transitionDuration
        onTriggered: actionDescription.showAction(actionDescription.nextActionText)
      }

      function showAction(description) {
        if (state == "hidden") {
          text  = description
          state = "shown"
          hideDescriptionTimer.restart()
        }
        else /* state == "shown" */ {
          nextActionText = description
          state = "hidden"
          showDescriptionTimer.restart()
        }
      }
    }

    MainText {
      id: imageSizePxLabel
      anchors {
        right: parent.right
        rightMargin: 5
        verticalCenter: statusBar.verticalCenter
      }
      text: ("%1x%2 px").arg(image.width).arg(image.height)
      color: "black"
    }
  }
}
