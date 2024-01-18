#ifndef CONN_CONFIG_H
#define CONN_CONFIG_H

#include <QNetworkInterface>
#include <expected>

class ConnConfigurator
{
protected:
    QHostAddress *ip;
    quint16 port;

public:
    virtual ~ConnConfigurator() {};
    virtual std::expected<std::pair<QString, quint16>, std::exception> retrieveConnectionConfiguration() = 0;
};

#endif
