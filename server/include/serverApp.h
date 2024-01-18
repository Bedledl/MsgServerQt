#ifndef SERVER_APP_H
#define SERVER_APP_H

#include <QWidget>

#include "server.h"

//#include "serverApp.moc"

class ServerApplication : public QWidget {
    Q_OBJECT

    public:
    ServerApplication(QWidget *parent = nullptr);
    private:
    Server *server = NULL;
    ChatParticipantOrganizer *chat_organizer = NULL;
};

#endif