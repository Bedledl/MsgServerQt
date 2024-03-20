#ifndef SERVER_INCLUDE_SERVER
#define SERVER_INCLUDE_SERVER
#include "chat.h"
#include "serverIface.h"
#include "serverParticipant.h"
#include <QHostAddress>
#include <QObject>
#include <exception>
#include <memory>

class TCPMessageServer;
class Worker;
// https://doc.qt.io/qt-6/qtnetwork-threadedfortuneserver-example.html

class ServerFailedToStart : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Failed to start server.";
    }
};

class Server : public ServerIface
{
public:
    Server(QHostAddress ip, quint16 port, bool usePingCommunicator, QObject *parent);
    virtual ~Server();
    std::shared_ptr<ServerChat> getGlobalChat() const
    {
        return globalChat;
    }
    void removeParticipantFromChat(ChatKey key, ParticipantKey participantKey) const override;
    void addParticipantToChat(std::shared_ptr<ServerChat> chat, ParticipantKey participantKey) const override;
    std::string getParticipantName(ParticipantKey key) const override;
    void setParticipantName(ParticipantKey key, std::string name) const override;
    QDateTime getParticipantEntryDate(ParticipantKey key) const override;
    void setParticipantEntryDate(ParticipantKey key, QDateTime entryDate) const override;
    void registerParticipant(std::shared_ptr<ServerParticipant> participant) override;
    void unregisterParticipant(ParticipantKey key) override;

protected:
    std::shared_ptr<ServerChat> globalChat;
    std::map<ParticipantKey, std::shared_ptr<ServerParticipant>> participants;
    std::unique_ptr<TCPMessageServer> tcpServer;
};

#endif /* SERVER_INCLUDE_SERVER */
