#ifndef COMMUNICATORS_H
#define COMMUNICATORS_H
#include <QString>

/// @brief Bridge between Client thread and the internal Logic, but can also be used for debugging
class Communicator {
    public:
    virtual QString answerMessage(QString msg) = 0;
};

class PingPongCommunicator : public Communicator {
    public:
    QString answerMessage(QString msg) override {
        if (msg == "Ping") {
            return "Pong";
        }
        else if (msg == "Pong") {
            return "Ping";
        }
        return "Don't you want to play Ping-Pong?";
    }
};

#endif
