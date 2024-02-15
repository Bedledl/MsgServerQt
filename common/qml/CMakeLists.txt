qt_add_library(commonGui STATIC)
set_source_files_properties(Constants.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)
qt_add_qml_module(commonGui
    URI "common.qml"
    VERSION 1.0
    QML_FILES
        Chat.qml Constants.qml
)
