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
                             auto expect_conn_details = config.retrieveConnectionConfiguration();
                             if (!expect_conn_details.has_value())
                             {
                                 throw ClientFailedToConnect();
                             }
                             auto &[ip, port] = expect_conn_details.value();
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