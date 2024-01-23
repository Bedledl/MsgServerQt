#ifndef COMMUNICATORS_H
#define COMMUNICATORS_H

#include <QString>

/// @brief Bridge between Client thread and the internal Logic, but can also be used for debugging
class Communicator
{
public:
    virtual QString answerMessage(QString msg) = 0;
    /// @brief Message that is sent from the Server to start communication after succesfully connecting to client.
    /// @return
    virtual QString welcomeMessage() = 0;
};

class PingPongCommunicator : public Communicator
{
public:
    QString answerMessage(QString msg) override
    {
        if (msg == "Ping")
        {
            return "Pong";
        }
        else if (msg == "Pong")
        {
            return "Ping";
        }
        return "Don't you want to play Ping-Pong?";
    }

    QString welcomeMessage() override
    {
        return QString("Ping");
    }
};

#endif