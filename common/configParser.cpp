#include "configParser.h"

#include "connConfigFile.h"
#include "connConfigGraphic.h"
#include "connConfigurator.h"
#include <QApplication>
#include <QCommandLineParser>
#include <map>
#include <variant>

void configureParser(QCommandLineParser &parser, QString configFileCmdLineOptName)
{
    parser.setApplicationDescription("Chat Program Command Line Parser");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configFileOption(QStringList() << configFileCmdLineOptName,
                                        QCoreApplication::translate("main", "Specfiy config file to configure connection with."),
                                        QCoreApplication::translate("main", "directory"));
    QCommandLineOption pingOption(QStringList() << "ping", QCoreApplication::translate("main", "Enable pinging between server and client for debugging."));
    parser.addOption(configFileOption);
    parser.addOption(pingOption);
}

std::unique_ptr<ConnConfigurator> createConnConfiguratorFromSettings(QCommandLineParser &parser, QString configFileCmdLineOptName)
{
    QString configFilePath = parser.value(configFileCmdLineOptName);
    if (configFilePath.isEmpty())
    {
        return std::make_unique<ConnConfiguratorGraphic>();
    }
    QFile file(configFilePath);
    return std::make_unique<ConnFileConfigurator>(file);
}

bool parsePingConfig(QCommandLineParser &parser, QString pingingModeCmdLineOptName)
{
    return parser.isSet(pingingModeCmdLineOptName);
}
