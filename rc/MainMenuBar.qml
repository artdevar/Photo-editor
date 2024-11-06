import QtQuick
import QtQuick.Controls
import Custom

MenuBar {
  id: root
  font {
    family: "SF Pro Text";
    pixelSize: 13
  }

  property Component brightnessEditorComponent: BrightnessContrastSliders {}
  property Component hueSaturationEditorComponent: HueSaturationSliders {}
  property Component histogramComponent: Histogram {}
  property Component gaussianBlurComponent: GaussianBlurSlider {}

  property bool optionsAvailable: false

  Menu {
    title: qsTr("File")
    font: root.font

    Action {
      text: qsTr("Open")
      shortcut: StandardKey.Open
      enabled: true
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandImageOpen {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    Action {
      text: qsTr("Save")
      shortcut: StandardKey.Save
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandImageSave {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    Action {
      text: qsTr("Save As")
      shortcut: StandardKey.SaveAs
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandImageSaveAs {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    MenuSeparator {}
    Action {
      text: qsTr("Close")
      shortcut: StandardKey.Close
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandImageClose {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
  }

  Menu {
    title: qsTr("Edit")
    font: root.font
    enabled: root.optionsAvailable

    Action {
      text: qsTr("Undo")
      shortcut: StandardKey.Undo
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandUndo {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    Action {
      text: qsTr("Redo")
      shortcut: StandardKey.Redo
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandRedo {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    MenuSeparator {}
    Action {
      text: qsTr("Flip vertical")
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandFlipImageVertical {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    Action {
      text: qsTr("Flip horizontal")
      enabled: root.optionsAvailable
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandFlipImageHorizontal {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
  }

  Menu {
    Loader { id: brightnessEditorLoader }
    Loader { id: hueSaturationEditorLoader }
    Loader { id: histogramLoader }

    title: qsTr("Correction")
    font: root.font
    enabled: root.optionsAvailable

    Action {
      text: qsTr("Brightness/Contrast")
      onTriggered: {
        brightnessEditorLoader.sourceComponent = root.brightnessEditorComponent
        brightnessEditorLoader.item.show()
        brightnessEditorLoader.item.closing.connect(function() { brightnessEditorLoader.sourceComponent = undefined })
      }
    }
    Action {
      text: qsTr("Hue/Saturation")
      onTriggered: {
        hueSaturationEditorLoader.sourceComponent = root.hueSaturationEditorComponent
        hueSaturationEditorLoader.item.show()
        hueSaturationEditorLoader.item.closing.connect(function() { hueSaturationEditorLoader.sourceComponent = undefined })
      }
    }
    Action {
      text: qsTr("Levels")
      onTriggered: {
        histogramLoader.sourceComponent = root.histogramComponent
        histogramLoader.item.show()
        histogramLoader.item.closing.connect(function() { histogramLoader.sourceComponent = undefined })
      }
    }
    MenuSeparator {}
    Action {
      text: qsTr("Auto contrast")
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandAutoContrast {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    Action {
      text: qsTr("Invert")
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandInvertPixels {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    Action {
      text: qsTr("Grayscale")
      onTriggered: {
        var command = Qt.createQmlObject('import Custom; CommandGrayscale {editorPtr: editor}', root)
        editor.executeInstantCommand(command)
      }
    }
    //Action {
    //  text: qsTr("Sepia")
    //  onTriggered: editor.actionSepia()
    //}
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
