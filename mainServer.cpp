#include "chat.h"
#include "configParser.h"
#include "connConfigGraphic.h"
#include "server.h"
#include <QApplication>
#include <iostream>

#include "connConfigFile.h"
#include <QCommandLineParser>
#include <QDir>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Server");
    QApplication::setApplicationName("LaunchServer");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    configureParser(parser);
    parser.process(app);

    std::unique_ptr<ConnConfigurator> connConfigurator = createConnConfiguratorFromSettings(parser);
    bool usePingCommunicator = parsePingConfig(parser);

    // may block until conneciton configuration is retrieved
    auto [ip, port] = connConfigurator->retrieveConnectionConfiguration();

    std::unique_ptr<Server> server = std::make_unique<Server>(QHostAddress(ip), port, usePingCommunicator, app.parent());

    return app.exec();
}
