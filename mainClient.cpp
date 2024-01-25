#include "connConfigGraphic.h"
#include <QApplication>

#include "client.h"
#include "clientApp.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Client");

    ConnConfiguratorGraphic config;
    config.show();

    std::unique_ptr<ClientApp> clientApp;

    QObject::connect(&config, &ConnConfiguratorGraphic::configReady,
                     [&]()
                     {
                         try
                         {
                             auto [ip, port] = config.retrieveConnectionConfiguration();
                             clientApp = std::make_unique<ClientApp>(std::make_unique<Client>(QHostAddress(ip), port));
                         }
                         catch (ClientFailedToConnect &exc)
                         {
                             config.displayError(exc);
                             return;
                         }
                         config.hide();
                         clientApp->show();
                     });

    return app.exec();
}