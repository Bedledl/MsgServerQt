#include "chat.h"
#include "configParser.h"
#include "connConfigGraphic.h"
#include "server/include/serverApp.h"
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

    QCommandLineParser parser;
    configureParser(parser);
    parser.process(app);

    std::unique_ptr<ConnConfigurator> connConfigurator = createConnConfiguratorFromSettings(parser);

    // may block until conneciton configuration is retrieved
    auto [ip, port] = connConfigurator->retrieveConnectionConfiguration();

    std::unique_ptr<ServerApplication> serverApp;
    std::unique_ptr<TCPMessageServer> server = std::make_unique<TCPMessageServer>(QHostAddress(ip), port);
    auto globalChat = server->getGlobalChat();

    try
    {
        serverApp = std::make_unique<ServerApplication>(std::move(server));
    }
    catch (ServerFailedToStart &exc)
    {
        //  config.displayError(exc);
        return EXIT_FAILURE;
    }

    serverApp->add_chat(new ChatGui(serverApp.get(), globalChat.get()));

    serverApp->show();

    return app.exec();
}
