#include "configParser.h"
#include "connConfigGraphic.h"
#include <QApplication>
#include <QCommandLineParser>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QDebug>

#include "client.h"
#include "clientApp.h"



int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    // qputenv("QT_LOGGING_RULES", "qt.qml.connections=false");
    // qputenv("QML_COMPAT_RESOLVE_URLS_ON_ASSIGNMENT", "1");

    //    QQuickView *view = new QQuickView;
    //    view->setSource(QUrl::fromLocalFile("../client/qml/Client.qml"));
    //    view->show();

     QApplication a(argc, argv);

     //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);


     QQmlApplicationEngine engine;
     engine.load(QUrl(QStringLiteral("../client/qml/Client.qml")));
     if (engine.rootObjects().isEmpty())
         return -1;

     return a.exec();

     qDebug() << "created url";
     // QObject::connect(
     //     &engine,
     //     &QQmlApplicationEngine::objectCreated,
     //     &appGui,
     //     [url](QObject *obj, const QUrl &objUrl) {
     //         if (!obj && url == objUrl) {
     //             qDebug() <<"!obj or url!=objUrl";
     //             QCoreApplication::exit(-1);
     //         }
     //     },
     //     Qt::QueuedConnection);

    // engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    // engine.addImportPath(":/");

     qDebug() << "this should never be reached";

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

    //clientApp->show();

    return app.exec();
}
