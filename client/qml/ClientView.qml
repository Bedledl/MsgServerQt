import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Window 2.15
//import QtQuick.Shapes

import common.qml


Window {
    id: clientWindow
    minimumHeight: 500
    minimumWidth: 250

    visible: true
    color: Constants.backgroundColor
    title: qsTr("Chat Client")


    property bool chatVisible: false

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10; anchors.leftMargin: 10; anchors.rightMargin: 10
        anchors.bottomMargin: 0
        spacing: 2
        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Hello " + clientCppObj.nickname
            color: Constants.secondColor
            font.pixelSize: 12
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Connected to server at IP <b>"+ clientCppObj.ip +"</b> on port <b>" + clientCppObj.port + "</b>"
            color: Constants.mainColor
            font.pixelSize: 10
        }



        ColumnLayout {
            id: smallLayout
            ChatPreviewList {}
            ChatView {}
        }

        RowLayout {
            id: largeLayout
            ChatPreviewList {}
            ChatView {}
        }
    }


    //! [setting the layout]
    function setFittingLayout() {
        if (width < 450) {
            // this should be made with loading probably
            // we don't want to have both layouts simulatiously. Is the layout there when it is not visible???
            smallLayout.visible = true
            largeLayout.visible = false
        } else {
            smallLayout.visible = false
            largeLayout.visible = true
        }
    }
    onWidthChanged: setFittingLayout()
    Component.onCompleted: setFittingLayout()
    //! [setting the layout]

}
