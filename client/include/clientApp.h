#ifndef CLIENT_APP_H
#define CLIENT_APP_H

#include "appGui.h"

class Client;

class ClientApp : public AppGui
{
    Q_OBJECT
    std::unique_ptr<Client> client;

public:
    ClientApp(std::unique_ptr<Client> client, QWidget *parent = nullptr);
};

#endif
