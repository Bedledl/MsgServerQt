#include <QLabel>
#include <QWidget>

#include <QVBoxLayout>

#include <QNetworkInterface>
#include <memory>

#include <QDebug>

#include "moc_serverApp.cpp"
#include "serverApp.h"
#include "tcpserver.h"

#include <iostream>

// much code here is copied from the fortuneclient example of qt

ServerApplication::ServerApplication(std::unique_ptr<Server> server, QWidget *parent) : AppGui(parent), server(std::move(server))
{
    status_label = std::make_unique<QLabel>();
    status_label->setWordWrap(true);
    status_label->setText("Server is running:)");

    hostCombo = std::make_unique<QComboBox>();
    hostCombo->setEditable(true);
    hostCombo->addItem("blub");
    hostCombo->addItem("blub2");

    auto ifaces_list = QNetworkInterface::allInterfaces();
    for (auto &iface : ifaces_list)
    {
        qDebug() << iface.name();
        auto address_list = iface.addressEntries();
        for (auto &entry : address_list)
        {
            qDebug() << entry.ip().toString();
        }
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(&(*status_label));
    mainLayout->addWidget(hostCombo.get());
    setLayout(mainLayout);
};
