import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import common.qml

Rectangle {
    id: chatPreviewRect
    width: parent ? parent.width : 0
    height: chatPreviewColumnLayout.height + 12
    clip: true
    color: chatPreviewClickingArea.containsMouse ? Constants.secondColor : Constants.mainColor

    // TODO borders instead of  second rectangle ?
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
                    text: model.display.chatName
                    color: chatPreviewRect.color
                    font.pointSize: 12
                    textFormat: Text.PlainText
                }
                GradientText {
                    id: lastMessage
                    text: model.display.lastMessageText
                    fontSize: 12
                    maxWidth: parent.width
                    //color: chatPreviewRect.color TODO
                }

                Text {
                    id: lastMessageDateString
                    text: model.display.lastMessageTimestamp.toDateString()
                    color: chatPreviewRect.color
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
            console.log(index)
            clientCppObj.selectedChat = model.display.chatKey
            clientWindow.chatVisible=true
        }
        id: chatPreviewClickingArea
    }
}
