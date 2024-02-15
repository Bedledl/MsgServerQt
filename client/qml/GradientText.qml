import QtQuick 2.15
import common.qml
Item {
    property string text: "Example Text, This is an exampleshgjsdfhgkdfjhgjkgh"
    property int fontSize: 12
    property int maxWidth: 200
    Rectangle {
        id: gradientRect
        width: chatPreviewColumnLayout.parent.width;height: textItem.height
        gradient: Gradient {
            GradientStop { position: 0; color: Constants.mainColor }
            GradientStop { position: 0.5; color: Constants.mainColor }
            GradientStop { position: 1; color: Constants.backgroundColor }
            orientation: Gradient.Horizontal
        }
        visible: false
        layer.enabled: true
        layer.smooth: false
    }

    Text {
        id: textItem
        font.pixelSize: parent.fontSize
        text: parent.text
        textFormat: Text.PlainText
        layer.enabled: true
        layer.samplerName: "maskSource"
        layer.effect: ShaderEffect {
            property var colorSource: gradientRect
            fragmentShader: "mask.frag.qsb"
        }
       }
    height: textItem.height
}
