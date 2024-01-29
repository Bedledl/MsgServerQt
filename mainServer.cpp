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

#include <QtGlobal>
#include <QByteArray>

int main(int argc, char *argv[])
{
    // Needed to enable logging to syslog or journald.
    qputenv("QT_LOGGING_TO_CONSOLE", QByteArray("0"));
    qDebug("Debug log message from Qt test program");

    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Server");
    QApplication::setApplicationName("LaunchServer");
    QApplication::setApplicationVersion("1.0");

    QString configFileCmdLineOptName = "config-file";
    QCommandLineParser parser;
    configureParser(parser, configFileCmdLineOptName);
    parser.process(app);

    std::unique_ptr<ConnConfigurator> connConfigurator = createConnConfiguratorFromSettings(parser, configFileCmdLineOptName);

    // may block until conneciton configuration is retrieved
    auto [ip, port] = connConfigurator->retrieveConnectionConfiguration();

    std::unique_ptr<ServerApplication> serverApp;

    try
    {
        serverApp = std::make_unique<ServerApplication>(std::make_unique<TCPMessageServer>(QHostAddress(ip), port));
    }
    catch (ServerFailedToStart &exc)
    {
        //  config.displayError(exc);
        return EXIT_FAILURE;
    }

    serverApp->add_chat(new ChatGui(serverApp.get(), GlobalChat::get_instance()));

    serverApp->show();

    return app.exec();
}
