import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Custom

PopupWindow {
  id: root
  title: qsTr("Adjust brightness/contrast")

  property var command: CommandChangeBrightnessContrast {
    editorPtr: editor
  }

  function onValuesChanged(brightness, contrast) {
    command.setBrightnessContrast(brightness, contrast)
    editor.executeIntermediateCommand(command)
  }

  ColumnLayout {
    anchors.fill: parent
    spacing: -5

    MainText {
      id: brightnessText
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.topMargin: 7
      text: qsTr("Brightness: %1").arg(brightnessSlider.value)
      color: "black"
      font.pointSize: root.fontSize
    }
    Slider {
      id: brightnessSlider
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: root.width * 0.8
      from: -255
      value: 0
      to: 255
      stepSize: 1
      onValueChanged: root.onValuesChanged(brightnessSlider.value, contrastSlider.value)
    }
    MainText {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      text: qsTr("Contrast: %1").arg(contrastSlider.value)
      color: "black"
      font.pointSize: root.fontSize
    }
    Slider {
      id: contrastSlider
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: root.width * 0.8
      value: 1.0
      from: 0.0
      to: 15.0
      stepSize: 0.05
      onValueChanged: root.onValuesChanged(brightnessSlider.value, contrastSlider.value)
    }
    RowLayout {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
      Layout.bottomMargin: 7
      spacing: 30
      Button {
        text: qsTr("OK")
        font: brightnessText.font
        Layout.preferredWidth: cancelButton.width
        onClicked: {
          editor.onIntermediateCommandExecuted(command)
          root.close()
        }
      }
      Button {
        id: cancelButton
        text: qsTr("CANCEL")
        font: brightnessText.font
        onClicked: {
          command.cancel()
          root.close()
        }
      }
    }
  }
}
