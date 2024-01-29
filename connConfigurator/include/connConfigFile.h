#ifndef BBF44417_C973_4BC3_9EFB_8ACC369AAEA6
#define BBF44417_C973_4BC3_9EFB_8ACC369AAEA6

#include "connConfigurator.h"
#include "libconfig.h++"
#include <QDir>

/// @brief Base Class for Exceptions that are associated with reading of the connection config file.
class ConfigFileException : public std::exception
{
protected:
    const std::string file_path;
    std::string further_information;
    virtual const char *what() const throw() = 0;

public:
    ConfigFileException(std::string file_path, std::string further_information = "") : file_path(file_path), further_information(further_information){};
};

/// @brief Exception that is thrown if a file is not found.
class FileNotFound : public ConfigFileException
{
    virtual const char *what() const throw()
    {
        std::string msg = std::string("File not found: ") + file_path + "(" + further_information + ")";
        return msg.c_str();
    }
    using ConfigFileException::ConfigFileException;
};

class FileIOException : public ConfigFileException
{
    virtual const char *what() const throw()
    {
        std::string msg = std::string("I/O Error while reading file: ") + file_path + "(" + further_information + ")";
        return msg.c_str();
    }
    using ConfigFileException::ConfigFileException;
};

class FileParsingException : public ConfigFileException
{
    virtual const char *what() const throw()
    {
        std::string msg = std::string("Error while parsing file: ") + file_path + "(" + further_information + ")";
        return msg.c_str();
    }
    using ConfigFileException::ConfigFileException;
};

/// @brief Looks for a conn.conf in the specified directory and reads the required information(iface, ip, port)
class ConnFileConfigurator : public ConnConfigurator
{
    libconfig::Config cfg;
    QString config_file_name;

public:
    ConnFileConfigurator(QDir config_dir, QString file_name);
    ConnFileConfigurator(QFile &file);
    std::pair<QString, quint16> retrieveConnectionConfiguration();

private:
    void init_file();
    std::string lookup_variable(std::string var);
};

#endif /* BBF44417_C973_4BC3_9EFB_8ACC369AAEA6 */
