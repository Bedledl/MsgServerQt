#ifndef CONN_CONFIG_GRAPHIC_H
#define CONN_CONFIG_GRAPHIC_H
#include <QComboBox>
#include <QDialog>
#include <QNetworkInterface>

#include "connConfigurator.h"

class ConnConfiguratorGraphic : public QDialog, public ConnConfigurator
{
    Q_OBJECT

    std::unique_ptr<QComboBox> ifaceBox;
    std::unique_ptr<QComboBox> addrBox;
    std::unique_ptr<QLineEdit> portLineEdit;
    std::unique_ptr<QPushButton> connectButton;

private slots:
    void ifaceSet(int index);
    void tryEnableConnectButton();

signals:
    void configReady();

public:
    ConnConfiguratorGraphic(QWidget *parent = nullptr);
    ~ConnConfiguratorGraphic();
    std::pair<QString, quint16> retrieveConnectionConfiguration() override;
public slots:
    void displayError(std::exception &exc);
};

#endif