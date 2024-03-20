#ifndef COMMUNICATORS_H
#define COMMUNICATORS_H

#include <QString>

/// @brief Bridge between Client thread and the internal Logic, but can also be used for debugging
class Communicator
{
public:
    Communicator(std::function<void(std::string)> sendFunc) : sendFunc(sendFunc) {}
    virtual void processRawMessage(std::string msg) = 0;
    void processRawMessage(const QString &msg)
    {
        processRawMessage(msg.toStdString());
    }
    void send(std::string msg)
    {
        sendFunc(std::move(msg));
    }

private:
    std::function<void(std::string)> sendFunc;
};

class PingPongCommunicator : public Communicator
{
public:
    explicit PingPongCommunicator(std::function<void(std::string)> sendAnswer) : Communicator(std::move(sendAnswer)) {}
    void processRawMessage(std::string msg) override
    {
        if (msg == "Ping")
        {
            send("Pong");
            return;
        }
        if (msg == "Pong")
        {
            send("Ping");
            return;
        }
        send("Don't you want to play Ping-Pong?");
    }
};

#endif
