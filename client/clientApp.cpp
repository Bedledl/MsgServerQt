#include "clientApp.h"
#include "client.h"
#include "moc_clientApp.cpp"

ClientApp::ClientApp(std::unique_ptr<Client> client, QWidget *parent) : AppGui(parent), client(std::move(client)) {}
