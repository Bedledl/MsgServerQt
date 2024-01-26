#include "connConfigFile.h"
#include "libconfig.h++"
#include <iostream>

ConnFileConfigurator::ConnFileConfigurator(QDir config_dir, QString file_name)
{
    if (!config_dir.exists(file_name))
    {
        QString file_path = config_dir.path() + file_name;
        throw FileNotFound(file_path.toStdString());
    }
    config_file_name = config_dir.filePath(file_name);
    init_file();
}

ConnFileConfigurator::ConnFileConfigurator(QFile &file)
{
    if (!file.exists())
    {
        throw FileNotFound(file.fileName().toStdString());
    }
    config_file_name = file.fileName();
    init_file();
}

std::pair<QString, quint16> ConnFileConfigurator::retrieveConnectionConfiguration()
{
    QString ip = QString(lookup_variable("IP").c_str());
    quint16 port = QString(lookup_variable("PORT").c_str()).toUShort();
    return {ip, port};
}

void ConnFileConfigurator::init_file()
{
    try
    {
        std::string file_name_std = config_file_name.toStdString();
        cfg.readFile(file_name_std.c_str());
    }
    catch (const libconfig::FileIOException &fioex)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        throw FileIOException(config_file_name.toStdString());
    }
    catch (const libconfig::ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        throw FileParsingException(pex.getFile());
    }
}

std::string ConnFileConfigurator::lookup_variable(std::string var)
{
    try
    {
        return cfg.lookup(var);
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        throw FileParsingException(config_file_name.toStdString(), var);
    }
}