#include "chat.h"
#include "configParser.h"
#include "connConfigGraphic.h"
#include "tcpserver.h"
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

    QString configFileCmdLineOptName = "config-file";
    QString pingingModeCmdLineOptName = "ping";
    QCommandLineParser parser;
    configureParser(parser, configFileCmdLineOptName);
    parser.process(app);

    std::unique_ptr<ConnConfigurator> connConfigurator = createConnConfiguratorFromSettings(parser, configFileCmdLineOptName);
    bool usePingCommunicator = parsePingConfig(parser, pingingModeCmdLineOptName);

    // may block until conneciton configuration is retrieved
    auto [ip, port] = connConfigurator->retrieveConnectionConfiguration();

    std::unique_ptr<TCPMessageServer> server = std::make_unique<TCPMessageServer>(QHostAddress(ip), port, usePingCommunicator);
    auto globalChat = server->getGlobalChat();

    return app.exec();
}
