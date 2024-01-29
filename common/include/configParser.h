#ifndef BB2F71AD_99F1_4905_98E2_89738BFFB13A
#define BB2F71AD_99F1_4905_98E2_89738BFFB13A

#include "connConfigurator.h"
#include <QString>
#include <memory>
QT_BEGIN_NAMESPACE
class QCommandLineParser;
QT_END_NAMESPACE

void configureParser(QCommandLineParser &parser, QString configFileCmdLineOptName);
std::unique_ptr<ConnConfigurator> createConnConfiguratorFromSettings(QCommandLineParser &parser, QString configFileCmdLineOptName);

#endif /* BB2F71AD_99F1_4905_98E2_89738BFFB13A */
