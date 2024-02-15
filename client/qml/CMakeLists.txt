qt_add_library(clientGui STATIC)
qt_add_qml_module(clientGui
    URI "common.qml"
    VERSION 1.0
    DEPENDENCIES commmon.qml
    QML_FILES
        ChatPreview.qml
        ChatView.qml
        GradientText.qml
        Client.qml
        Message.qml
        ChatPreviewList.qml
    RESOURCES
        mask.frag.qsb
)
