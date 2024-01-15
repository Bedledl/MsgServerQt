#include <QApplication>
#include "server/include/serverApp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Server");
    ServerApplication serverApp;
    serverApp.show();
    return app.exec();
}
