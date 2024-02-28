#ifndef SERVER_H
#define SERVER_H
#include "chat.h"
#include "participant.h"
#include "serverIface.h"
#include <exception>
#include <memory>

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
    Server()
    {
        globalChat = std::make_shared<ServerChat>();
    }
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
    void addNewIncomingMessage(ChatKey key, std::string content, ParticipantKey participantKey, QDateTime timestamp) const override;
    void registerParticipant(std::shared_ptr<ServerParticipant> participant);
    void unregisterParticipant(ParticipantKey key);

protected:
    void create_new_client_thread(Worker *worker);
    std::shared_ptr<ServerChat> globalChat;
    std::map<ParticipantKey, std::shared_ptr<ServerParticipant>> participants;
};

#endif