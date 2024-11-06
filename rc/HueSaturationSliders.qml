import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Custom

PopupWindow {
  id: root
  title: qsTr("Adjust hue/saturation/lightness")
  height: 220

  property var command: CommandChangeHueSaturationLight {
    editorPtr: editor
  }

  function onValuesChanged(hue, saturation, lightness) {
    command.setHueSaturationLightness(hue, saturation, lightness)
    editor.executeIntermediateCommand(command)
  }

  ColumnLayout {
    anchors.fill: parent
    spacing: -8

    MainText {
      id: hueText
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.topMargin: 7
      text: qsTr("Hue: %1").arg(hueSlider.value)
      color: "black"
      font.pointSize: root.fontSize
    }
    Slider {
      id: hueSlider
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: root.width * 0.8
      from: 0
      value: 0
      to: 360
      stepSize: 1
      onValueChanged: root.onValuesChanged(hueSlider.value, saturationSlider.value, lightnessSlider.value)
    }
    MainText {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.topMargin: 7
      text: qsTr("Saturation: %1").arg(saturationSlider.value)
      color: "black"
      font.pointSize: root.fontSize
    }
    Slider {
      id: saturationSlider
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: root.width * 0.8
      from: -100
      value: 0
      to: 100
      stepSize: 1
      onValueChanged: root.onValuesChanged(hueSlider.value, saturationSlider.value, lightnessSlider.value)
    }
    MainText {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.topMargin: 7
      text: qsTr("Lightness: %1").arg(lightnessSlider.value)
      color: "black"
      font.pointSize: root.fontSize
    }
    Slider {
      id: lightnessSlider
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: root.width * 0.8
      from: -100
      value: 0
      to: 100
      stepSize: 1
      onValueChanged: root.onValuesChanged(hueSlider.value, saturationSlider.value, lightnessSlider.value)
    }
    RowLayout {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
      Layout.bottomMargin: 7
      spacing: 30
      Button {
        text: qsTr("OK")
        font: hueText.font
        Layout.preferredWidth: cancelButton.width
        onClicked: {
          editor.onIntermediateCommandExecuted(command)
          root.close()
        }
      }
      Button {
        id: cancelButton
        text: qsTr("CANCEL")
        font: hueText.font
        onClicked: {
          command.cancel()
          root.close()
        }
      }
    }
  }
}
