import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
//import QtQuick.Shapes

import common.qml


Item {
    width: 400; height: 800

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10; anchors.leftMargin: 10; anchors.rightMargin: 10
        anchors.bottomMargin: 0
        spacing: 2
        ChatPreview {}
        Label {
            text: "Hello User" //TODO fill with real name
        }

        Label {
            text: "Connected to server at IP <b>foobar</b> on port <b>666</b>"
        }

        ListView {
            id: chatList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: 20
            delegate: ChatPreview {}
            focus: true
            spacing: 2
        }

    }

}
