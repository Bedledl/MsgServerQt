#ifndef BB2F71AD_99F1_4905_98E2_89738BFFB13A
#define BB2F71AD_99F1_4905_98E2_89738BFFB13A

#include <memory>
#include <string>

class QCommandLineParser;

class ConnConfigurator;

constexpr inline std::string configFileCmdLineOptName = "config-file";
constexpr inline std::string pingingModeCmdLineOptName = "ping";

void configureParser(QCommandLineParser &parser);
std::unique_ptr<ConnConfigurator> createConnConfiguratorFromSettings(QCommandLineParser &parser);
bool parsePingConfig(QCommandLineParser &parser);

#endif /* BB2F71AD_99F1_4905_98E2_89738BFFB13A */
