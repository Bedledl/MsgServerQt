import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import common.qml
Rectangle {
    id: rect
    width: chatList.width
    height: chatL.height + 4
    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        color: Constants.backgroundColor

        RowLayout {

        Rectangle {
            width: 10
            height: chatL.height
        }

        ColumnLayout {
            id: chatL
            spacing: 1
            Text {
                id: chatIdentification
                text: "Chat" + index
                color: Constants.mainColor
                font.pointSize: 14
            }
            GradientText {
                id: lastMessage
                text: "Hey, good to hear from you!jasklfjksljfksaldfjkfdljdafkjjklkjkl" + chatL.height
                // transform:
                fontSize: 12
            }

            Text {
                id: lastMessageDateString
                text: "Today, 7:34pm"
                color: Constants.mainColor
                font.pointSize: 8
            }
        }

        }

    }

    color: Constants.mainColor
}

