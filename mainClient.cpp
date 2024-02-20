#include "configParser.h"
#include "connConfigGraphic.h"
#include <QApplication>
#include <QCommandLineParser>

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "client.h"

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");

    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Client");
    QApplication::setApplicationName("LaunchClient");
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

    std::unique_ptr<Client> client;

    try
    {
        client = std::make_unique<Client>(QHostAddress(ip), port, usePingCommunicator);
    }
    catch (ClientFailedToConnect &exc)
    {
        return EXIT_FAILURE;
    }

    //    std::unique_ptr<ClientApp> clientApp;
    //
    //    try
    //    {
    //        clientApp = std::make_unique<ClientApp>(std::make_unique<Client>(QHostAddress(ip), port, usePingCommunicator));
    //    }
    //    catch (ClientFailedToConnect &exc)
    //    {
    //        // config.displayError(exc);
    //        return EXIT_FAILURE;
    //    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("clientCppObj", client.get());

    engine.load(QUrl(QStringLiteral("../client/qml/ClientView.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
