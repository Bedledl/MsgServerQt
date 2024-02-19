import QtQuick 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls 2.15

import common.qml

Rectangle {
    id: chatView
    border.color: Constants.mainColor
    border.width: 2
    color: Constants.backgroundColor

    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true
    visible: clientWindow.chatVisible
    Item {
        anchors.fill: parent
        anchors.margins: 2

        ColumnLayout {
            id: chatLayout2
            anchors.fill: parent
            spacing: 2
            Layout.margins: 2
            ListView {
                id: messageList
                boundsMovement: Flickable.StopAtBounds
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.verticalStretchFactor: 8
                snapMode: ListView.SnapToItem
                model: 100
                clip: true
                delegate: Message {}
                spacing: 2
                Component.onCompleted: positionViewAtEnd()
            }
            RowLayout {
                Layout.margins: 5
                TextArea {
                    id: newMessageField
                    Text {
                        text: "Type in your message!"
                        color: Constants.secondColor
                        visible: parent.text == ""
                        padding: parent.padding
                    }
                    padding: 10
                    Layout.maximumHeight: 100
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.verticalStretchFactor: 2
                    wrapMode: TextInput.Wrap

                    text: ""
                    color: Constants.mainColor

                    function accept() {
                        if (!text) {return}

                        // TODO
                        console.log(text)
                        clear()
                    }
                }

                RoundButton {
                    id: sendButton
                    text: String.fromCodePoint(0x27A3)
                    onClicked: newMessageField.accept()
                    font.pixelSize: 19
                }
            }
        }

        Rectangle {
            Text {
                id: closeChatButtonIcon
                text: largeLayout.visible ? String.fromCodePoint(0x2347) : String.fromCodePoint(0x2350)
                color: Constants.mainColor
                font.pixelSize: 19
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: clientWindow.chatVisible=false
            }

            states: [
                State {
                    name: "small"
                    when: smallLayout.visible
                    PropertyChanges {
                        target: closeChatButtonIcon
                        text: String.fromCodePoint(0x2350)
                    }
                    AnchorChanges {
                        target: closeChatButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top

                        anchors.left: undefined
                        anchors.verticalCenter: undefined
                    }
                },
                State {
                    name: "large"
                    when: largeLayout.visible
                    PropertyChanges {
                        target: closeChatButtonIcon
                        text: String.fromCodePoint(0x2347)
                    }
                    AnchorChanges {
                        target: closeChatButton
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left

                        anchors.horizontalCenter: undefined
                        anchors.top: undefined
                    }
                }
            ]

            id: closeChatButton
            width: closeChatButtonIcon.width
            height: closeChatButtonIcon.height

            color: Constants.backgroundColor
            opacity: 0.5
        }
    }
}
