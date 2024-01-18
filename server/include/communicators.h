#ifndef SERVER_COMMUNICATORS_H
#define SERVER_COMMUNICATORS_H
#include <QString>

#include "communicator.h"

class MessengerCommunicator : public Communicator
{
public:
    QString answerMessage(QString msg) override;
};

#endif
