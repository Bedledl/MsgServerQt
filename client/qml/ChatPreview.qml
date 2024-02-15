import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import common.qml

Rectangle {
    id: rect
    width: parent ? parent.width : 0
    height: chatPreviewColumnLayout.height + 12
    clip: true
    color: chatPreviewClickingArea.containsMouse ? Constants.secondColor : Constants.mainColor

    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        color: Constants.backgroundColor
        Item {
            anchors.fill: parent
            anchors.margins: 5

            ColumnLayout {
                Layout.fillWidth: true
                id: chatPreviewColumnLayout
                spacing: 2

                Text {
                    id: chatIdentification
                    text: "Chat" + index
                    color: Constants.mainColor
                    font.pointSize: 12
                    textFormat: Text.PlainText
                }
                GradientText {
                    id: lastMessage
                    text: "Hey, good to hear from you!jasklfjksljfksaldfjkfdljdafkjjklkjkl"
                    fontSize: 12
                    maxWidth: parent.width
                }

                Text {
                    id: lastMessageDateString
                    text: "Today, 7:34pm"
                    color: Constants.mainColor
                    font.pointSize: 8
                    textFormat: Text.PlainText
                }
            }
        }

    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onDoubleClicked:  {
            clientWindow.chatVisible=true
        }
        id: chatPreviewClickingArea
    }
}
