#include "chat.h"
#include "connConfigGraphic.h"
#include "server/include/serverApp.h"
#include "tcpserver.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("TCP Message Server");

    ConnConfiguratorGraphic config;
    config.show();

    std::unique_ptr<ServerApplication> serverApp;

    QObject::connect(&config, &ConnConfiguratorGraphic::configReady,
                     [&]()
                     {
                         try
                         {
                             auto expect_conn_details = config.retrieveConnectionConfiguration();
                             if (!expect_conn_details.has_value())
                             {
                                 throw ServerFailedToStart();
                             }
                             auto &[ip, port] = expect_conn_details.value();
                             serverApp = std::make_unique<ServerApplication>(std::make_unique<TCPMessageServer>(QHostAddress(ip), port));
                         }
                         catch (ServerFailedToStart &exc)
                         {
                             config.displayError(exc);
                             return;
                         }
                         config.hide();

                         serverApp->add_chat(new ChatGui(serverApp.get(), GlobalChat::get_instance()));

                         serverApp->show();
                     });

    return app.exec();
}
