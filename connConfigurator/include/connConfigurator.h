#ifndef CONN_CONFIG_H
#define CONN_CONFIG_H

#include <QNetworkInterface>

class ConnConfigurator
{
protected:
    QHostAddress *ip;
    quint16 port;

public:
    virtual ~ConnConfigurator(){};
    virtual std::pair<QString, quint16> retrieveConnectionConfiguration() = 0;
};

#endif
