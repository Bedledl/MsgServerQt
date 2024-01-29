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
    parser.addOption(configFileOption);
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
