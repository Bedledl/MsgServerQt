import QtQuick 2.15
import QtQuick.Layouts 2.15


ListView {
    id: chatList
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true
    model: clientCppObj.CPLmodel
    delegate: ChatPreview {}
    focus: true
    spacing: 2
}
