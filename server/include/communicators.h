#include <QString>

/// @brief Bridge between Client thread and the internal Logic, but can also be used for debugging
class Communicator {
    public:
    QString answerMessage(QString msg);
};

class PingPongCommunicator {
    QString answerMessage(QString msg) {
        if (msg == "Ping") {
            return "Pong";
        }
        else if (msg == "Pong") {
            return "Ping";
        }
        return "Don't you want to play Ping-Pong?";
    }
};