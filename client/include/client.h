#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QMap>
#include <QObject>

#include "chat.h"
#include "communicator.h"
#include "participant.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
QT_END_NAMESPACE

class ClientFailedToConnect : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Failed to connect.";
    }
};

class ParticipantNotFound : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Participant not found.";
    }
};

class ChatNotFound : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Chat not found.";
    }
};

class ChatAlreadyExists : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Chat already exists.";
    }
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QHostAddress ip, quint16 port, bool pingMode, QObject *parent = nullptr);
    void addNewChat(const ChatKey &key)
    {
        if (chats.contains(key))
        {
            return;
        }
        auto newChat = new Chat(key, this);
        chats.insert(key, newChat);
    }
    void leaveChat(const ChatKey &key)
    {
        auto countDeleted = chats.remove(key);
        if (countDeleted == 0)
        {
            throw ChatNotFound();
        }
    }
    void addNewIncomingMessage(const ChatKey &key, QString content, ParticipantKey participantKey, QDateTime timestamp)
    {
        if (auto searchChat = chats.find(key); searchChat != chats.end())
        {
            if (auto searchParticipant = registeredParticipants.find(participantKey); searchParticipant != registeredParticipants.end())
            {
                (*searchChat)->addMessage(std::move(content), *searchParticipant, std::move(timestamp));
            }
            else
            {
                throw ParticipantNotFound();
            }
        }
        else
        {
            throw ChatNotFound();
        }
    }
    void assignParticipantName(const ParticipantKey &key, QString name)
    {
        if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
        {
            (*search)->setNickname(name);
        }
    }
    void assignParticipantEntryDate(const ParticipantKey &key, QDateTime entryDate)
    {
        if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
        {
            (*search)->setEntryDate(std::move(entryDate));
        }
    }

private slots:
    void readFromSocketAndAswer();

private:
    QLabel *statusLabel = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
    std::unique_ptr<Communicator> communicator;
    QMap<ChatKey, Chat *> chats;
    QMap<ParticipantKey, std::shared_ptr<Participant>> registeredParticipants;
};

#endif
