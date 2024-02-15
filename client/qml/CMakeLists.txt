qt_add_library(clientGui STATIC)
qt_add_qml_module(clientGui
    URI "common.qml"
    VERSION 1.0
    DEPENDENCIES commmon.qml
    QML_FILES ChatPreview.qml
    QML_FILES GradientText.qml Client.qml
    RESOURCES mask.frag.qsb
    QML_FILES ChatView.qml
    QML_FILES Message.qml
    QML_FILES ChatPreviewList.qml
)
