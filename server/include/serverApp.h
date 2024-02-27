#ifndef SERVER_APP_H
#define SERVER_APP_H

#include <QComboBox>
#include <QLabel>
#include <QWidget>

#include "appGui.h"

class Server;

class ServerApplication : public AppGui
{
    Q_OBJECT

public:
    ServerApplication(std::unique_ptr<Server> server, QWidget *parent = nullptr);

private:
    std::unique_ptr<Server> server;
    std::unique_ptr<QLabel> status_label;
    std::unique_ptr<QComboBox> hostCombo;
};

#endif