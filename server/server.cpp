#include <algorithm>
#include <map>
#include <vector>

#include "clientThreadWorker.h"
#include "server.h"
#include "tcpserver.h"

Server::Server(QHostAddress ip, quint16 port, bool usePingCommunicator, QObject *parent)
{
    globalChat = std::make_shared<ServerChat>();
    tcpServer = std::make_unique<TCPMessageServer>(*this, ip, port, usePingCommunicator, parent);
}

Server::~Server() {}

void Server::registerParticipant(std::shared_ptr<ServerParticipant> participant)
{
    if (participants.contains(participant->getKey()))
    {
        throw ParticipantAlreadyExists();
    }
    else
    {
        participants.emplace(participant->getKey(), std::move(participant));
    }
}
void Server::unregisterParticipant(ParticipantKey key)
{
    auto countRemoved = participants.erase(key);
    if (countRemoved == 0)
    {
        throw ParticipantNotFound();
    }
}
void Server::removeParticipantFromChat(ChatKey key, ParticipantKey participantKey) const
{
    auto participant = participants.find(participantKey);
    if (participant != participants.end())
    {
        (*participant).second->leaveChat(key);
    }
    else
    {
        throw ParticipantNotFound();
    }
}
void Server::addParticipantToChat(std::shared_ptr<ServerChat> chat, ParticipantKey participantKey) const
{
    auto participant = participants.find(participantKey);
    if (participant != participants.end())
    {
        (*participant).second->joinChat(std::move(chat));
    }
    else
    {
        throw ParticipantNotFound();
    }
}
std::string Server::getParticipantName(ParticipantKey key) const
{
    auto participant = participants.find(key);
    if (participant != participants.end())
    {
        return (*participant).second->getNickname().toStdString();
    }
    else
    {
        throw ParticipantNotFound();
    }
}
void Server::setParticipantName(ParticipantKey key, std::string name) const
{
    auto participant = participants.find(key);
    if (participant != participants.end())
    {
        (*participant).second->setNickname(QString::fromStdString(name));
    }
    else
    {
        throw ParticipantNotFound();
    }
}
QDateTime Server::getParticipantEntryDate(ParticipantKey key) const
{
    auto participant = participants.find(key);
    if (participant != participants.end())
    {
        return (*participant).second->getEntryDate();
    }
    else
    {
        throw ParticipantNotFound();
    }
}
void Server::setParticipantEntryDate(ParticipantKey key, QDateTime entryDate) const
{
    auto participant = participants.find(key);
    if (participant != participants.end())
    {
        (*participant).second->setEntryDate(entryDate);
    }
    else
    {
        throw ParticipantNotFound();
    }
}
