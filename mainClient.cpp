#include "configParser.h"
#include "connConfigGraphic.h"
#include <QApplication>
#include <QCommandLineParser>

#include "client.h"
#include "clientApp.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Client");
    QApplication::setApplicationName("LaunchClient");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    configureParser(parser);
    parser.process(app);

    std::unique_ptr<ConnConfigurator> connConfigurator = createConnConfiguratorFromSettings(parser);
    bool usePingCommunicator = parsePingConfig(parser);

    // may block until conneciton configuration is retrieved
    auto [ip, port] = connConfigurator->retrieveConnectionConfiguration();

    std::unique_ptr<ClientApp> clientApp;

    try
    {
        clientApp = std::make_unique<ClientApp>(std::make_unique<Client>(QHostAddress(ip), port, "Juliet", usePingCommunicator));
    }
    catch (ClientFailedToConnect &exc)
    {
        // config.displayError(exc);
        return EXIT_FAILURE;
    }

    clientApp->show();

    return app.exec();
}