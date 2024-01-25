
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVariant>

#include "connConfigGraphic.h"
#include "connConfigurator.h"
#include "moc_connConfigGraphic.cpp"

void ConnConfiguratorGraphic::ifaceSet(int index)
{
    const QString iface_name = ifaceBox->itemText(index);

    auto iface_obj = QNetworkInterface::interfaceFromName(iface_name);

    addrBox->clear();

    if (!iface_obj.isValid())
    {
        return;
    }

    auto address_list = iface_obj.addressEntries();
    for (const auto &entry : address_list)
    {
        qDebug() << entry.ip().toString();
        addrBox->addItem(entry.ip().toString());
    }
}

ConnConfiguratorGraphic::ConnConfiguratorGraphic(QWidget *parent) : QDialog(parent), ConnConfigurator()
{
    ifaceBox = std::make_unique<QComboBox>();
    addrBox = std::make_unique<QComboBox>();
    portLineEdit = std::make_unique<QLineEdit>();
    connectButton = std::make_unique<QPushButton>("Connect");

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    connect(ifaceBox.get(), &QComboBox::currentIndexChanged, this, &ConnConfiguratorGraphic::ifaceSet);

    auto ifaces_list = QNetworkInterface::allInterfaces();
    for (auto &iface : ifaces_list)
    {
        qDebug() << iface.name();
        ifaceBox->addItem(iface.name());
    }

    connectButton->setDefault(true);
    connectButton->setEnabled(false);
    connect(connectButton.get(), &QAbstractButton::clicked, this, &ConnConfiguratorGraphic::configReady);

    connect(portLineEdit.get(), &QLineEdit::textChanged,
            this, &ConnConfiguratorGraphic::tryEnableConnectButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(ifaceBox.get());
    mainLayout->addWidget(addrBox.get());
    mainLayout->addWidget(portLineEdit.get());
    mainLayout->addWidget(connectButton.get());
    setLayout(mainLayout);
};

ConnConfiguratorGraphic::~ConnConfiguratorGraphic(){};

std::pair<QString, quint16> ConnConfiguratorGraphic::retrieveConnectionConfiguration()
{
    return std::pair{addrBox->currentText(), portLineEdit->text().toInt()};
}

void ConnConfiguratorGraphic::tryEnableConnectButton()
{
    connectButton->setEnabled(!addrBox->currentText().isEmpty() && !portLineEdit->text().isEmpty());
}

void ConnConfiguratorGraphic::displayError(std::exception &exc)
{
    QMessageBox::information(this, tr("Error"), tr(exc.what()));
}
