import QtQuick 2.15
import QtQuick.Layouts 2.15

import common.qml

Rectangle {
    radius: 10
    color: Constants.secondColor
    height: 30
    implicitWidth: msgContent.width + 10
    anchors.left: (msgContent.incoming && parent) ? parent.left : undefined
    anchors.right: (msgContent.incoming) ? undefined : (parent ? parent.right: undefined)
    Text {
        property bool incoming: true
        anchors.centerIn: parent
        id: msgContent
        text: "This is a mesage" + index
        color: Constants.mainColor
        textFormat: Text.PlainText
    }
}
